#include "label_rules.h"
#include "label_solver.h"

void TestApplyRule() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("-phi"));
  n.root.CreateLabel(false, Formula("-phi"));
  auto result = ApplyRule(n, LRNotPlus, true);
  assert(result.size() == 1);
  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(-2) == Formula("phi"));
  assert(result[0].used_rules.size() == 1);
  assert(result[0].used_rules.count(make_pair(1, LRNotPlus)) == 1);
}

void TestSolveTermination() {
  LabelNode n;
  LabelSolve(n);
  assert(n.is_closed.value() == false);

  n.root.CreateLabel(true, Formula("phi"));
  n.root.CreateLabel(true, Formula("psi"));
  n.root.MakeEqual(1, 2);
  n.root.MakeNotEqual(2, 1);
  LabelSolve(n);
  assert(n.is_closed.value() == true);

  n = LabelNode();
  n.root.CreateLabel(true, Formula("phi"));
  n.root.CreateLabel(false, Formula("psi"));
  n.root.MakeEqual(1, -1);
  LabelSolve(n);
  assert(n.is_closed.value() == true);
}

int main() {
  TestApplyRule();
  TestSolveTermination();
}