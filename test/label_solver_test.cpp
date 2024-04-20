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

void TestCompare() {
  TestInput("p");
  TestInput("p,q, -(p=q)");
  TestInput("p,q,-p,-(p=q)");
  TestInput(
      "v12, v10, v9, v6, v4, v3, r, q, -p, -v8, -v11, (p=p), (v4=v3), "
      "(v8=-v9), (v11=-v10), (v11=-v12), (v6=(p->r)), (v12=(p->p)), "
      "(v7=(p->v8)), (v5=(p=q)), (v4=(v5->v6)), (v4=(v7=p)), (v9=(v10->v11))");
  TestInput(
      "q, ¬p, ¬v3, ¬v4,(v5≡(q≡p)), v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput("q, v8, ¬p, ¬v3, ¬v4, ¬v9, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), "
            "(v12≡(p→p)), (v7≡(p→v8)), (v6≡(p→r)), (v5≡(q≡p)), (v3≡(p≡v7)), "
            "(v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput(
      "q, ¬p, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), (v6≡(p→r)), (v12≡(p→p)), "
      "(v7≡(p→v8)), (v5≡(p≡q)), (v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput("-((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <>"
            "(p = p)) | ((p & p) | -q)))");
  TestInput("¬(((b→b)→b)→b)");
  TestInput("b≡((b↔b)∧¬b)");
  TestInput("a≡¬¬(a→¬a)");
  // TestInput("a≡¬((b↔a)↔b)");
}

int main() {
  TestApplyRule();
  TestSolveTermination();
  TestCompare();
}