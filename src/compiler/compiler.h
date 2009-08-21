#ifndef COMPILER_H
#define COMPILER_H

#include <clang/Basic/LangOptions.h>
#include <clang/Frontend/CompileOptions.h>

namespace llvm {
   class Module;
}

class Compiler
{
public:
   Compiler();
   ~Compiler();

   llvm::Module *compile(const std::string &text);

private:
   void init();

private:
   clang::LangOptions m_langOptions;
   clang::CompileOptions m_compileOptions;
};

#endif
