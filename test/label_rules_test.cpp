#include "label_rules.h"

void TestLRNotPlus() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("-p"));

  auto result = LRNotPlus(n, 1);
  assert(result.size() == 1);
  assert(result[0].root.GetFormula(-1) == Formula("p"));

  n.root.CreateLabel(true, Formula("p"));
  result = LRNotPlus(n, 2);
  assert(result.size() == 0);
}
int main() { TestLRNotPlus(); }