#include "commons.h"
#include "rules.h"
#include "solver.h"
#include "structs.h"

Formula f_not("--phi");
Formula f_impl("phi->psi");
Formula f_not_impl("-(phi->psi)");

void testRNot() {
  assert(RNot(Formula(5)).empty());
  assert(RNot(Formula(op_not, {5})).empty());

  assert(RNot(f_not) == vector<Set>{Set({Formula(0)})});
}

void testRImpl() {
  assert(RImpl(f_not).empty());
  assert(RImpl(f_impl) == ParseSets("-phi|psi"));
  assert(RImpl(f_not_impl).empty());
}
void testRNotImpl() {
  assert(RNotImpl(f_not_impl) == ParseSets("phi, -psi"));
}

int main() {
  testRNot();
  testRImpl();
  testRNotImpl();
}