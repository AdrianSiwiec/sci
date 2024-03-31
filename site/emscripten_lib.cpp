#include <stdio.h>
#include <emscripten.h>
#include "commons.h"
#include "solver.h"
#include "structs.h"

extern "C" {
EMSCRIPTEN_KEEPALIVE
void sayHi(char * name) {
  cout<<"Hello there!"<<endl;
  cout<<"General "<<name<<"!"<<endl;
}

void SolveFormulas(char *formulas) {
  Set input_set(formulas);
  ProofNode n(input_set);
  IsClosed(n);
  PrintProofNode(n);
}
}