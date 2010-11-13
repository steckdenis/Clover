#ifndef COAL_COMPILER_H
#define COAL_COMPILER_H

#include <clang/Frontend/CompilerInstance.h>

namespace llvm {
   class Module;
}

namespace Coal {
   class Compiler
   {
   public:
      Compiler();
      ~Compiler();

      llvm::Module *compile(const std::string &text);

   private:
      bool init();

   private:
      clang::CompilerInstance m_clang;
   };
}

#endif
