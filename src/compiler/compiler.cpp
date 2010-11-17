#include "compiler.h"

#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Tool.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/DiagnosticOptions.h>
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
#include <llvm/System/Host.h>
#include <llvm/System/Path.h>
#include <llvm/Target/TargetSelect.h>

using namespace Coal;
using namespace clang;


Compiler::Compiler()
{
   init();
}

Compiler::~Compiler()
{

}

bool Compiler::init()
{
}

llvm::Module * Compiler::compile(const std::string &text)
{
   clang::TextDiagnosticPrinter *DiagClient =
      new clang::TextDiagnosticPrinter(llvm::errs(), DiagnosticOptions());

   clang::Diagnostic Diags(DiagClient);
   clang::driver::Driver TheDriver(
      "clc", llvm::sys::getHostTriple(),
      "a.out", /*IsProduction=*/false, /*CXXIsProduction=*/false,
      Diags);
   TheDriver.setTitle("OpenCL Interpreter");

   // FIXME: This is a hack to try to force the driver to do something we can
   // recognize. We need to extend the driver library to support this use model
   // (basically, exactly one input, and the operation mode is hard wired).
   llvm::SmallVector<const char *, 16> Args(4);
   Args[0] = "clc";
   Args[1] = "-fsyntax-only";
   Args[2] = "-v";
   Args[3] = "/home/zack/main.c";
   llvm::OwningPtr<clang::driver::Compilation> C(
      TheDriver.BuildCompilation(Args.size(), Args.data()));
   if (!C)
      return 0;

   // FIXME: This is copied from ASTUnit.cpp; simplify and eliminate.

   // We expect to get back exactly one command job, if we didn't something
   // failed. Extract that job from the compilation.
   const driver::JobList &Jobs = C->getJobs();
   if (Jobs.size() != 1 || !isa<driver::Command>(Jobs.begin())) {
      llvm::SmallString<256> Msg;
      llvm::raw_svector_ostream OS(Msg);
      C->PrintJob(OS, C->getJobs(), "; ", true);
      Diags.Report(diag::err_fe_expected_compiler_job) << OS.str();
      return 0;
   }

   const driver::Command *Cmd = cast<driver::Command>(*Jobs.begin());
   if (llvm::StringRef(Cmd->getCreator().getName()) != "clang") {
      Diags.Report(diag::err_fe_expected_clang_command);
      return 0;
   }

   // Initialize a compiler invocation object from the clang (-cc1) arguments.
   const driver::ArgStringList &CCArgs = Cmd->getArguments();
   llvm::OwningPtr<CompilerInvocation> CI(new CompilerInvocation);
   CompilerInvocation::CreateFromArgs(*CI,
                                      const_cast<const char **>(CCArgs.data()),
                                      const_cast<const char **>(CCArgs.data()) +
                                      CCArgs.size(),
                                      Diags);

   // Show the invocation, with -v.
   if (CI->getHeaderSearchOpts().Verbose) {
      llvm::errs() << "clang invocation:\n";
      C->PrintJob(llvm::errs(), C->getJobs(), "\n", true);
      llvm::errs() << "\n";
   }

   // FIXME: This is copied from cc1_main.cpp; simplify and eliminate.

   // Create a compiler instance to handle the actual work.
   CompilerInstance Clang;
   Clang.setLLVMContext(new llvm::LLVMContext);
   Clang.setInvocation(CI.take());

   // Create the compilers actual diagnostics engine.
   Clang.createDiagnostics(int(CCArgs.size()),const_cast<char**>(CCArgs.data()));
   if (!Clang.hasDiagnostics())
      return 0;

   // Infer the builtin include path if unspecified.
   if (Clang.getHeaderSearchOpts().UseBuiltinIncludes &&
       Clang.getHeaderSearchOpts().ResourceDir.empty()) {
      assert(0);
      //Clang.getHeaderSearchOpts().ResourceDir =
      //   CompilerInvocation::GetResourcesPath(argv[0], MainAddr);
   }

   // Create and execute the frontend to generate an LLVM bitcode module.
   llvm::OwningPtr<CodeGenAction> Act(new EmitLLVMOnlyAction());
   if (!Clang.ExecuteAction(*Act))
      return 0;

   return Act->takeModule();
}
