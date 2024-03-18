#include "solver.h"

void TestSimple() {
  ProofNode node(Set({}));
  PopulateSubnodes(node);
  assert(node.subnodes.empty());
}

int main() { TestSimple(); }