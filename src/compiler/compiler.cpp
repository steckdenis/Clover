#include "compiler.h"

#include <clang/Lex/Preprocessor.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/InitPreprocessor.h>
#include <clang/Frontend/InitHeaderSearch.h>
#include <clang/Frontend/Utils.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Sema/ParseAST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/SourceLocation.h>

#include <llvm/Module.h>
#include <llvm/ADT/OwningPtr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/System/Host.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/MemoryBuffer.h>

using namespace clang;

static void llvmErrorHandler(void *userData, const std::string &message)
{
   clang::Diagnostic &diags = *static_cast<clang::Diagnostic*>(userData);

   diags.Report(clang::FullSourceLoc(), clang::diag::err_fe_error_backend) << message;

   // We cannot recover from llvm errors.
   exit(1);
}

static void
initializePreprocessorInitOptions(clang::PreprocessorInitOptions &InitOpts)
{
   //add macros and paths
}

/* almost verbatim copy of the class from clang-cc.cpp */
namespace {
class DriverPreprocessorFactory : public PreprocessorFactory {
   clang::Diagnostic        &Diags;
   const clang::LangOptions &LangInfo;
   clang::TargetInfo        &Target;
   clang::SourceManager     &SourceMgr;
   clang::HeaderSearch      &HeaderInfo;

public:
   DriverPreprocessorFactory(clang::Diagnostic &diags, const clang::LangOptions &opts,
                             clang::TargetInfo &target, clang::SourceManager &SM,
                             clang::HeaderSearch &Headers)
      : Diags(diags), LangInfo(opts), Target(target),
        SourceMgr(SM), HeaderInfo(Headers)
   {}

   virtual ~DriverPreprocessorFactory()
   {}

   virtual Preprocessor * CreatePreprocessor() {
      llvm::OwningPtr<clang::PTHManager> PTHMgr;

      if (Diags.hasErrorOccurred())
         exit(1);

      // Create the Preprocessor.
      llvm::OwningPtr<clang::Preprocessor> PP(
         new clang::Preprocessor(Diags, LangInfo, Target,
                                 SourceMgr, HeaderInfo,
                                 PTHMgr.get()));

      // Note that this is different then passing PTHMgr to Preprocessor's ctor.
      // That argument is used as the IdentifierInfoLookup argument to
      // IdentifierTable's ctor.
      if (PTHMgr) {
         PTHMgr->setPreprocessor(PP.get());
         PP->setPTHManager(PTHMgr.take());
      }

      clang::PreprocessorInitOptions InitOpts;
      initializePreprocessorInitOptions(InitOpts);
      if (clang::InitializePreprocessor(*PP, InitOpts))
         return 0;

      return PP.take();
   }
};
}


static bool
initializeSourceManager(Preprocessor &pp,
                        const std::string &inFile,
                        const std::string &sourceStr)
{
   // Figure out where to get and map in the main file.
   SourceManager &sourceMgr = pp.getSourceManager();
   FileManager &fileMgr = pp.getFileManager();

   if (inFile != "-") {
      const FileEntry *file = fileMgr.getFile(inFile);
      if (file) sourceMgr.createMainFileID(file, SourceLocation());
      if (sourceMgr.getMainFileID().isInvalid()) {
         pp.getDiagnostics().Report(FullSourceLoc(), diag::err_fe_error_reading) 
            << inFile.c_str();
         return true;
      }
   } else {
      const char *source = sourceStr.c_str();
      llvm::MemoryBuffer *sb =
         llvm::MemoryBuffer::getMemBuffer(source,
                                          &source[sourceStr.size()],
                                          "OpenCL Source");

      sourceMgr.createMainFileIDForMemBuffer(sb);
      if (sourceMgr.getMainFileID().isInvalid()) {
         pp.getDiagnostics().Report(FullSourceLoc(),
                                    diag::err_fe_error_reading_stdin);
         return true;
      }
   }

   return false;
}

static void initializeIncludePaths(clang::HeaderSearch &headers,
                                   clang::FileManager &fm,
                                   const clang::LangOptions &lang)
{
   clang::InitHeaderSearch init(headers, true, "/");

   init.AddDefaultEnvVarPaths(lang);
   init.AddDefaultSystemIncludePaths(lang);

   init.Realize();
}

Compiler::Compiler()
{
   init();
}

Compiler::~Compiler()
{

}

void Compiler::init()
{
   m_langOptions.OpenCL = 1;
   m_langOptions.AltiVec = 1;
   m_langOptions.CXXOperatorNames = 1;
   m_langOptions.LaxVectorConversions = 1;
   m_langOptions.C99 = 1;
   m_langOptions.HexFloats = 1;
   m_langOptions.BCPLComment = 1;
   m_langOptions.Digraphs = 1;
   m_langOptions.Bool = 1;
}

llvm::Module * Compiler::compile(const std::string &text)
{
   llvm::OwningPtr<ASTConsumer> consumer;
   std::string moduleId;
   std::string llvmIr;
   llvm::raw_string_ostream output(llvmIr);
   llvm::OwningPtr<clang::DiagnosticClient> diagClient;
   std::vector<std::string> optWarnings;
   std::string targetTriple = llvm::sys::getHostTriple();
   llvm::OwningPtr<clang::SourceManager> sourceMgr;

   diagClient.reset(new clang::TextDiagnosticPrinter(llvm::errs()));
   clang::Diagnostic diags(diagClient.get());

   if (clang::ProcessWarningOptions(diags,
                                    optWarnings,
                                    true /*pedantic warnings*/,
                                    false /*OptPedanticErrors */,
                                    false /*OptNoWarnings*/))
      return 0;

   // Set an error handler, so that any LLVM backend diagnostics go through our
   // error handler.
   llvm::llvm_install_error_handler(llvmErrorHandler,
                                    static_cast<void*>(&diags));

   llvm::OwningPtr<clang::TargetInfo> target(
      clang::TargetInfo::CreateTargetInfo(targetTriple));

   if (target == 0) {
      diags.Report(clang::FullSourceLoc(), clang::diag::err_fe_unknown_triple)
         << targetTriple.c_str();
      return 0;
   }

   llvm::StringMap<bool> features;
   target->getDefaultFeatures(m_targetCpu, features);

   if (!sourceMgr)
      sourceMgr.reset(new clang::SourceManager());
   else
      sourceMgr->clearIDTables();

   diagClient->setLangOptions(&m_langOptions);

   // Allow the target to set the default the langauge options as it sees fit.
   target->getDefaultLangOptions(m_langOptions);

   // Pass the map of target features to the target for validation and
   // processing.
   target->HandleTargetFeatures(features);

   clang::FileManager fileMgr;
   clang::HeaderSearch headerInfo(fileMgr);
   initializeIncludePaths(headerInfo, fileMgr, m_langOptions);

   // Set up the preprocessor with these options.
   DriverPreprocessorFactory ppFactory(diags, m_langOptions, *target,
                                       *sourceMgr.get(), headerInfo);

   llvm::OwningPtr<Preprocessor> pp(ppFactory.CreatePreprocessor());

   if (!pp)
      return 0;

   if (initializeSourceManager(*pp.get(), std::string(), text))
      return 0;

   pp->getBuiltinInfo().InitializeBuiltins(pp->getIdentifierTable(),
                                           pp->getLangOptions().NoBuiltin);

   consumer.reset(clang::CreateBackendConsumer(
                     clang::Backend_EmitLL,
                     pp->getDiagnostics(),
                     pp->getLangOptions(),
                     m_compileOptions,
                     moduleId,
                     &output,
                     llvm::getGlobalContext()));

   llvm::OwningPtr<ASTContext> contextOwner;
   if (consumer)
      contextOwner.reset(new ASTContext(pp->getLangOptions(),
                                        pp->getSourceManager(),
                                        pp->getTargetInfo(),
                                        pp->getIdentifierTable(),
                                        pp->getSelectorTable(),
                                        pp->getBuiltinInfo(),
                                        /* FreeMemory = */ true,
                                        /* size_reserve = */0));

   if (consumer)
      ParseAST(*pp.get(), consumer.get(), *contextOwner.get(),
               false, true);

   consumer.reset();
   contextOwner.reset();

   return 0;
}
