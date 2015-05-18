#include <iostream>
#include "TSystem.h"


void minimain(bool silent = false){  

  gSystem->CompileMacro("minitree.C","k");
  if(!silent){
    std::cout << "[Info:] Macro compiled " << std::endl;
  }
}
