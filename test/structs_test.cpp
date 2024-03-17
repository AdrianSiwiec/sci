#include "commons.h"
#include "structs.h"

void FormulaTest() {
  Formula f(4);
  assert(f.IsVar(4));
  assert(!f.IsVar(5));
  assert(!f.IsOp(op_not));
}

int main() {
  FormulaTest();
}