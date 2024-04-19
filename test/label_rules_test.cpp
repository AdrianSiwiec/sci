#include "label_rules.h"

void TestLRNotPlus() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("-p"));

  auto result = LRNotPlus(n, 1);
  assert(result.size() == 1);
  assert(result[0].root.GetLabels().size() == 2);
  assert(result[0].root.GetFormula(-1) == Formula("p"));

  n.root.CreateLabel(true, Formula("p"));
  result = LRNotPlus(n, 2);
  assert(result.size() == 0);
}

void TestLRNotMinus() {
  LabelNode n;
  n.root.CreateLabel(false, Formula("-p"));

  auto result = LRNotMinus(n, -1);
  assert(result.size() == 1);
  assert(result[0].root.GetLabels().size() == 2);
  assert(result[0].root.GetFormula(1) == Formula("p"));

  n.root.CreateLabel(false, Formula("p"));
  result = LRNotPlus(n, -2);
  assert(result.size() == 0);
}

void TestLRImplPlus() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p->q"));

  auto result = LRImplPlus(n, 1);
  assert(result.size() == 3);

  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(-1) == Formula("p"));
  assert(result[0].root.GetFormula(-2) == Formula("q"));

  assert(result[1].root.GetLabels().size() == 3);
  assert(result[1].root.GetFormula(-1) == Formula("p"));
  assert(result[1].root.GetFormula(2) == Formula("q"));

  assert(result[2].root.GetLabels().size() == 3);
  assert(result[2].root.GetFormula(2) == Formula("p"));
  assert(result[2].root.GetFormula(3) == Formula("q"));
}

void TestLRImplMinus() {
  LabelNode n;
  n.root.CreateLabel(false, Formula("p->q"));

  auto result = LRImplMinus(n, -1);
  assert(result.size() == 1);
  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(1) == Formula("p"));
  assert(result[0].root.GetFormula(-2) == Formula("q"));
}

void TestLRIdPlus() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p=q"));

  auto result = LRIdPlus(n, 1);
  assert(result.size() == 2);

  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(2) == Formula("p"));
  assert(result[0].root.GetFormula(3) == Formula("q"));
  assert(result[0].root.IsEqual(2, 3));

  assert(result[1].root.GetLabels().size() == 3);
  assert(result[1].root.GetFormula(-1) == Formula("p"));
  assert(result[1].root.GetFormula(-2) == Formula("q"));
  assert(result[1].root.IsEqual(-1, -2));
}

void TestLRIdMinus() {
  LabelNode n;
  n.root.CreateLabel(false, Formula("p=q"));

  auto result = LRIdMinus(n, -1);
  assert(result.size() == 4);

  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(1) == Formula("p"));
  assert(result[0].root.GetFormula(2) == Formula("q"));
  assert(result[0].root.IsNotEqual(1, 2));

  assert(result[1].root.GetLabels().size() == 3);
  assert(result[1].root.GetFormula(1) == Formula("p"));
  assert(result[1].root.GetFormula(-2) == Formula("q"));

  assert(result[2].root.GetLabels().size() == 3);
  assert(result[2].root.GetFormula(-2) == Formula("p"));
  assert(result[2].root.GetFormula(1) == Formula("q"));

  assert(result[3].root.GetLabels().size() == 3);
  assert(result[3].root.GetFormula(-2) == Formula("p"));
  assert(result[3].root.GetFormula(-3) == Formula("q"));
  assert(result[3].root.IsNotEqual(-2, -3));
}

void TestLERNot() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p"));
  n.root.CreateLabel(true, Formula("q"));
  n.root.CreateLabel(true, Formula("-p"));
  n.root.CreateLabel(true, Formula("-q"));

  assert(LERNot(n) == false);

  n.root.MakeEqual(1, 2);
  assert(LERNot(n) == true);
  assert(n.root.IsEqual(3, 4));

  assert(LERNot(n) == false);
}

void TestLERImpl() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p"));
  n.root.CreateLabel(true, Formula("q"));
  n.root.CreateLabel(true, Formula("r"));
  n.root.CreateLabel(true, Formula("s"));
  n.root.CreateLabel(true, Formula("p->q"));
  n.root.CreateLabel(true, Formula("r->s"));
  n.root.MakeEqual(1, 3);
  assert(LERImpl(n) == false);
  n.root.MakeEqual(2, 4);
  assert(LERImpl(n) == true);
  assert(n.root.IsEqual(5, 6));
}

void TestLEREq() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p"));
  n.root.CreateLabel(true, Formula("q"));
  n.root.CreateLabel(true, Formula("r"));
  n.root.CreateLabel(true, Formula("s"));
  n.root.CreateLabel(true, Formula("p=q"));
  n.root.CreateLabel(true, Formula("r=s"));
  n.root.MakeEqual(1, 3);
  assert(LEREq(n) == false);
  n.root.MakeEqual(2, 4);
  assert(LEREq(n) == true);
  assert(n.root.IsEqual(5, 6));
}

void TestLERF() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("p"));
  n.root.CreateLabel(true, Formula("p"));
  n.root.CreateLabel(true, Formula("q"));
  assert(LERF(n) == true);
  assert(LERF(n) == false);
  assert(n.root.IsEqual(1, 2));
}

int main() {
  TestLRNotPlus();
  TestLRNotMinus();
  TestLRImplPlus();
  TestLRImplMinus();
  TestLRIdPlus();
  TestLRIdMinus();

  TestLERNot();
  TestLERImpl();
  TestLEREq();
  TestLERF();
}