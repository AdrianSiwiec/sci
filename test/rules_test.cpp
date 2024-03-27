#include "commons.h"
#include "preprocessing.h"
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
void testRNotImpl() { assert(RNotImpl(f_not_impl) == ParseSets("phi, -psi")); }

void testReplaceAll() {
  assert(ReplaceAll(Formula("phi->psi"), Formula("phi"), Formula("psi")) ==
         Formula("psi->psi"));
  assert(ReplaceAll(Formula("-phi->psi"), Formula("phi"), Formula("psi")) ==
         Formula("-psi->psi"));
  assert(ReplaceAll(Formula("phi->psi"), Formula("phi->psi"), Formula("psi")) ==
         Formula("psi"));
  assert(ReplaceAll(Formula("-(phi->psi)"), Formula("phi->psi"),
                    Formula("psi")) == Formula("-psi"));
}
void testRFun() {
  vector<Set> sets = RFun(Formula("phi=-psi"), Set("-psi,phi=-psi,psi"));
  assert(RFun(Formula("phi=-psi"), Set("-psi,phi=-psi,psi")) ==
         vector<Set>{Set("phi,psi,phi=-psi")});
  assert(RFun(Formula("phi=psi"), Set("phi=psi, psi->psi, psi->phi")) ==
         ParseSets("phi=psi, phi->phi"));
}
void testGetNewVar() {
  int var = GetNewVar();
  assert(var == 2);
  assert(int_to_variable[var] == "v2");
  assert(variable_to_int["v2"] = 2);
}

void testNEq1() {
  // First calculate...
  auto result = RNEq1(Formula("-(phi=(phi->psi))"));
  // Then parse the answer, otherwise it creates v4
  assert(result == ParseSets("-(phi=v3),v3=(phi->psi)"));

  assert(RNEq1(Formula("-(-phi=-phi)")).empty());
}

void testNEq2() {
  ClearVars();
  // First calculate...
  auto result = RNEq2(Formula("-(phi=(phi->psi))"));
  // Then parse the answer, otherwise it creates v4
  assert(result == ParseSets("(phi=v2),v3=(phi->psi),-(v2=v3)"));
}

int main() {
  testRNot();
  testRImpl();
  testRNotImpl();
  testReplaceAll();
  testRFun();
  testGetNewVar();
  testNEq1();
  testNEq2();
}