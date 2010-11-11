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

void Compiler::init()
{
#if 0
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
   Clang.createDiagnostics(int(CCArgs.size()),const_cast<char**>(
                              CCArgs.data()));
   if (!Clang.hasDiagnostics())
      return 1;

   // Infer the builtin include path if unspecified.
   if (Clang.getHeaderSearchOpts().UseBuiltinIncludes &&
       Clang.getHeaderSearchOpts().ResourceDir.empty())
      Clang.getHeaderSearchOpts().ResourceDir =
         CompilerInvocation::GetResourcesPath(argv[0], MainAddr);
#endif
}

llvm::Module * Compiler::compile(const std::string &text)
{
   // Create and execute the frontend to generate an LLVM bitcode module.
   llvm::OwningPtr<CodeGenAction> act(new EmitLLVMOnlyAction());
   if (!m_clang.ExecuteAction(*act))
      return 0;

   return act->takeModule();
}
