#include "compiler.h"

#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/DiagnosticOptions.h>
#include <clang/Frontend/HeaderSearchOptions.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Config/config.h>
#include <llvm/ADT/OwningPtr.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Config/config.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Path.h>
#include <llvm/Target/TargetSelect.h>

#include <iostream>
#include <fstream>

using namespace Coal;
using namespace clang;

static void
setupCodeGenOpts(CodeGenOptions &opts)
{
   opts.DebugInfo = true;
   //opts.OptimizationLevel = 2;
   //opts.MainFileName = "main.cl";
   //opts.UnrollLoops = true;
   opts.AsmVerbose = true;
}

static void
setupDiagnosticOpts(DiagnosticOptions &opts)
{
   opts.Pedantic = true;
   opts.ShowColumn = true;
   opts.ShowLocation = true;
   opts.ShowCarets = true;
   opts.ShowFixits = true;
   opts.ShowColors = true;
   opts.ErrorLimit = 19;
   opts.MessageLength = 80;
   //opts.Warnings[0] = "all";
}

static void
setupFrontendOpts(FrontendOptions &opts, const std::string &fileLoc)
{
   opts.ProgramAction = frontend::EmitLLVMOnly;
   opts.DisableFree = true;

   /* XXX HACK
    * just matches whatever garbage Compiler::prepareInput
    * creates */
   opts.Inputs.push_back(
      std::make_pair(IK_OpenCL, fileLoc));
}

static void
setupHeaderSearchOpts(HeaderSearchOptions &opts)
{
   opts.Verbose = true;
   opts.UseBuiltinIncludes = false;
   opts.UseStandardIncludes = false;
   opts.UseStandardCXXIncludes = false;

   std::string buildPath(COAL_BUILD_DIR);
   buildPath += "/src/builtins";
   opts.AddPath(buildPath,
                frontend::Angled, true,
                /*IsFramework=*/ false, true);
   std::string installPath(COAL_INSTALL_DIR);
   installPath += "/include/coal";
   opts.AddPath(installPath,
                frontend::Angled, true,
                /*IsFramework=*/ false, true);
}

static void
setupLangOpts(LangOptions &opts)
{
   opts.NoBuiltin = true;
}

static void
setupPreprocessorOpts(PreprocessorOptions &opts)
{
   opts.Includes.push_back("coal-internal.h");
}

static void
setupTargetOpts(TargetOptions &opts)
{
   opts.Triple = llvm::sys::getHostTriple();
   //opts.CPU =
}


Compiler::Compiler()
{
   init();
}

Compiler::~Compiler()
{

}

bool Compiler::init()
{
   std::string tempDir = llvm::sys::Path::GetTemporaryDirectory().str();
   m_tempFileLocation = tempDir + "/main.cl";

   // Create the compilers actual diagnostics engine.
   m_clang.createDiagnostics(0 ,NULL);
   if (!m_clang.hasDiagnostics())
      return 0;

   setupHeaderSearchOpts(m_clang.getHeaderSearchOpts());
   setupPreprocessorOpts(m_clang.getPreprocessorOpts());
   setupCodeGenOpts(m_clang.getCodeGenOpts());
   setupDiagnosticOpts(m_clang.getDiagnosticOpts());
   setupFrontendOpts(m_clang.getFrontendOpts(), m_tempFileLocation);
   setupLangOpts(m_clang.getLangOpts());
   setupTargetOpts(m_clang.getTargetOpts());
}

llvm::Module * Compiler::compile(const std::string &text)
{
   llvm::Module *module;
   /* setup the input file */
   prepareInput(text);

   // Create and execute the frontend to generate an LLVM bitcode module.
   llvm::OwningPtr<CodeGenAction> act(new EmitLLVMOnlyAction(new llvm::LLVMContext));
   if (!m_clang.ExecuteAction(*act))
      return 0;

   module = act->takeModule();
   //module->dump();
   return module;
}


/** XXX
 *  this needs to go. it's a nasty, nasty hack.
 *  It's also a serious security threat!
 */
void Compiler::prepareInput(const std::string &text)
{
   std::string error;

   std::cerr << "Output path = " << m_tempFileLocation.c_str();
   std::cerr << "Prepare input = "<<text << std::endl;

   std::fstream stream(m_tempFileLocation.c_str(), std::ios::out | std::ios::trunc);
   stream << text;
   stream.close();
}
