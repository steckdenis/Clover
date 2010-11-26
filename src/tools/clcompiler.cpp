#include "compiler.h"

#include <iostream>
#include <fstream>

static
void usage(const char *progName)
{
   std::cout << "Usage: " << progName << " file.cl" << std::endl;
}

int main(int argc, char **argv)
{
   Coal::Compiler compiler;

   if (argc < 2) {
      usage(argv[0]);
      return 1;
   }
   std::ifstream stream(argv[1]);
   std::string contents((std::istreambuf_iterator<char>(stream)),
                        std::istreambuf_iterator<char>());

   compiler.compile(contents);

   return 0;
}
