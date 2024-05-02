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
  assert(RImpl(f_impl) == ParseSets("-phi}psi"));
  assert(RImpl(f_not_impl).empty());
}
void testRNotImpl() { assert(RNotImpl(f_not_impl) == ParseSets("phi, -psi")); }

void testSym() {
  ClearVars();
  assert(RSym1(Formula("p=q")).empty());
  assert(RSym1(Formula("q=p"))[0].Formulas()[0] == Formula("p=q"));
  assert(RSym2(Formula("-(p=q)")).empty());
  assert(RSym2(Formula("-(q=p)"))[0].Formulas()[0] == Formula("-(p=q)"));
}

void testReplaceAll() {
  Formula f("phi->psi");
  assert(ReplaceAll(f, Formula("phi"), Formula("psi")));
  assert(f == Formula("psi->psi"));

  f = Formula("-phi->psi");
  assert(ReplaceAll(f, Formula("phi"), Formula("psi")));
  assert(f == Formula("-psi->psi"));

  f = Formula("phi->psi");
  assert(ReplaceAll(f, Formula("phi->psi"), Formula("psi")));
  assert(f == Formula("psi"));

  f = Formula("-(phi->psi)");
  assert(ReplaceAll(f, Formula("phi->psi"), Formula("psi")));
  assert(f == Formula("-psi"));
}
void testRFun() {
  vector<Set> sets = RFun(Formula("phi=-psi"), Set("-psi,phi=-psi,psi"));
  assert(RFun(Formula("phi=-psi"), Set("-psi,phi=-psi,psi")) ==
         vector<Set>{Set("phi,psi, phi=-psi")});

  assert(RFun(Formula("phi=psi"), Set("phi=psi, psi->psi, psi->phi")) ==
         ParseSets("phi->phi, phi=psi"));
}
void testGetNewVar() {
  // Brittle
  int var = GetNewVar();
  assert(var == 4);
  assert(int_to_variable[var] == "v4");
  assert(variable_to_int["v4"] = 4);
}

void testIsSimple() {
  assert(IsSimple(Formula("p")));
  assert(IsSimple(Formula("-p")));
  assert(IsSimple(Formula("p=q")));
  assert(IsSimple(Formula("-(p=q)")));
  assert(!IsSimple(Formula("-p=q")));
  assert(IsSimple(Formula("p=-q")));
  assert(IsSimple(Formula("p=(p=p)")));
  assert(IsSimple(Formula("p=(p->p)")));

  assert(!IsSimple(Formula("--p")));
  assert(!IsSimple(Formula("-p=-q")));
  assert(!IsSimple(Formula("-(-p=q)")));
  assert(!IsSimple(Formula("p=(-p=p)")));
  assert(!IsSimple(Formula("p=(p=-p)")));
  assert(!IsSimple(Formula("p=(p->-p)")));
  assert(!IsSimple(Formula("p=(-p->p)")));
}

void testNEq1() {
  ClearVars();
  // First calculate...
  auto result = RNEq1(Formula("-(phi=(phi->psi))"));
  // Then parse the answer, otherwise it creates v4
  assert(result == ParseSets("-(phi=v2),v2=(phi->psi)"));

  assert(RNEq1(Formula("-(-phi=-phi)")).empty());
  assert(RNEq1(Formula("-(phi=psi)")).empty());
}

void testNEq2() {
  ClearVars();
  // First calculate...
  auto result = RNEq2(Formula("-(phi=(phi->psi))"));
  // Then parse the answer, otherwise it creates v4
  assert(result == ParseSets("(v2=phi),v3=(phi->psi),-(v2=v3)"));
  assert(RNEq1(Formula("-(phi=psi)")).empty());
}
void testREqNot() {
  ClearVars();
  auto result = REqNot(Formula("phi=--psi"));
  assert(result == ParseSets("phi=-v2, v2=-psi"));

  result = REqNot(Formula("phi=-(phi=psi)"));
  assert(result == ParseSets("phi=-v3, v3=(phi=psi)"));

  assert(REqNot(Formula("(phi=phi)=-(phi=psi)")).empty());
  assert(REqNot(Formula("phi=-psi")).empty());
}

void testREqImpl() {
  ClearVars();
  auto result = REqImpl(Formula("phi=(-phi->-psi)"));
  assert(result == ParseSets("phi=(v2->v3), v2=-phi, v3=-psi"));
  assert(REqImplLeft(Formula("phi=(-phi->-psi)")).empty());
  assert(REqImplRight(Formula("phi=(-phi->-psi)")).empty());

  assert(REqImpl(Formula("phi=(phi->psi)")).empty());

  assert(REqImpl(Formula("-phi=(-phi->-psi)")).empty());

  result = REqImplLeft(Formula("phi=(psi->-psi)"));
  assert(result == ParseSets("phi=(psi->v4), v4=-psi"));
  assert(REqImplRight(Formula("phi=(psi->-psi)")).empty());

  result = REqImplRight(Formula("phi=(-psi->psi)"));
  assert(result == ParseSets("phi=(v5->psi), v5=-psi"));
  assert(REqImplLeft(Formula("phi=(-psi->psi)")).empty());
}
void testREqEq() {
  ClearVars();
  auto result = REqEq(Formula("phi=(-phi=-psi)"));
  assert(result == ParseSets("phi=(v2=v3), v2=-phi, v3=-psi"));
  assert(REqEqLeft(Formula("phi=(-phi=-psi)")).empty());
  assert(REqEqRight(Formula("phi=(-phi=-psi)")).empty());

  assert(REqEq(Formula("phi=(phi=psi)")).empty());

  result = REqEqLeft(Formula("phi=(psi=-psi)"));
  assert(result == ParseSets("phi=(psi=v4), v4=-psi"));
  assert(REqEqRight(Formula("phi=(psi=-psi)")).empty());

  // In my normalized formulas, only simple formula can match REqEqRight
  // (because the variable is always on the left)
  // result = REqEqRight(Formula("phi=(-psi=psi)"));
  // assert(result == ParseSets("phi=(v5=psi), v5=-psi"));
  // assert(REqEqLeft(Formula("phi=(-psi=psi)")).empty());
}

void testREq() {
  ClearVars();
  auto result = REq(Formula("-phi=-psi"));
  assert(result == ParseSets("v2=v3, v2=-phi, v3=-psi"));

  assert(REq(Formula("phi=psi")).empty());
  assert(REq(Formula("phi=-psi")).empty());
}
void testRTerEq() {
  assert(RTerEq(Formula("(p=q)")) == ParseSets("p,q,p=q } -p, -q, p=q"));
  assert(RTerEqNot(Formula("p=-q")) == ParseSets("p,-q,p=-q } -p,q,p=-q"));
  assert(RTerEqImpl(Formula("p=(q->r)")) ==
         ParseSets("p,-q,p=(q->r) } p,r,p=(q->r) } -p,q,-r,p=(q->r)"));
  assert(RTerEqEq(Formula("p=(q=r)")) == ParseSets("\
                  p, q, r, q=r, p=(q=r) } \
                  p, -q,-r, q=r, p=(q=r) } \
                  -p, q, r, -(q=r), p=(q=r) } \
                  -p, q, -r, -(q=r), p=(q=r) } \
                  -p, -q, r, -(q=r), p=(q=r) } \
                  -p, -q, -r, -(q=r), p=(q=r)"));
  assert(RTerSpike(Formula("-(p=q)")) == ParseSets("\
                 p, q, -(p=q) } \
                 p, -q, -(p=q) } \
                 -p, q, -(p=q) } \
                 -p, -q, -(p=q)"));
}

void testApplyRule() {
  Formula f("p");
  Set s({f});
  set<pair<int, FormulaSet>> ar;
  FormulaSet fs(make_pair(f, s));
  assert(!WasRuleApplied(RTerSpike, fs, ar));
  MarkRuleAsApplied(RTerSpike, fs, ar);
  assert(WasRuleApplied(RTerSpike, fs, ar));
  assert(!WasRuleApplied(RTerSpike, make_pair(f, Set({Formula("q")})), ar));
  assert(!WasRuleApplied(RTerEqEq, fs, ar));
}

int main() {
  testRNot();
  testRImpl();
  testRNotImpl();
  testSym();
  testReplaceAll();
  testRFun();
  testGetNewVar();
  testIsSimple();
  testNEq1();
  testNEq2();
  testREqNot();
  testREqImpl();
  testREqEq();
  testREq();
  testRTerEq();
  testApplyRule();
}