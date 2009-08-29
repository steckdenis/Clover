#include "compiler.h"

#include <iostream>

static
void usage(const char *progName)
{
   std::cout << "Usage: " << progName << " file.cl" << std::endl;
}

int main(int argc, char **argv)
{
   Compiler compiler;

   if (argc < 2) {
      usage(argv[0]);
      return 1;
   }

   compiler.compile(argv[1]);

   return 0;
}
