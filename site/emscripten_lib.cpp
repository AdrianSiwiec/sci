#include "commons.h"
#include "label_solver.h"
#include "solver.h"
#include "structs.h"
#include <emscripten.h>
#include <stdio.h>

extern "C" {
EMSCRIPTEN_KEEPALIVE
void sayHi(char *name) {
  cout << "Hello there!" << endl;
  cout << "General " << name << "!" << endl;
}

void SolveFormulas(char *formulas) { DoSolve(formulas); }
void SolveFormulasLabel(char *formulas) { DoSolveLabel(formulas, true); }
}
