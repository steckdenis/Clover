#include "compiler.h"

#include <clang/Frontend/ASTConsumers.h>
#include <clang/Basic/Diagnostic.h>

#include <llvm/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/LLVMContext.h>

Compiler::Compiler()
{
   init();
}

Compiler::~Compiler()
{

}

void Compiler::init()
{
   m_langOptions.OpenCL = true;
}

llvm::Module * Compiler::compile(const std::string &text)
{
   clang::Diagnostic diags;
   clang::ASTConsumer *consumer = 0;
   std::string moduleId;
   std::string llvmIr;
   llvm::raw_string_ostream output(llvmIr);

   consumer = clang::CreateBackendConsumer(
      clang::Backend_EmitLL,
      diags,
      m_langOptions,
      m_compileOptions,
      moduleId,
      &output,
      llvm::getGlobalContext());

}
