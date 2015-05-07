#include <iostream>
#include "TSystem.h"


void main(bool silent = false){  

  gSystem->CompileMacro("code_gen.C","k");
  if(!silent){
    std::cout << "[Info:] Macro compiled " << std::endl;
  }
}
