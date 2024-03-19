#include "rules.h"
#include "solver.h"

void TestBuildChildNodes() {
  ProofNode n({{{1}, {2}, {3}}});

  vector<ProofNode> children =
      BuildChildNodes(n, 0, {Set({{9}, {2}, {5}}), Set({Formula(3)})});

  assert(children[0].root == Set({{2}, {3}, {5}, {9}}));
  assert(children[1].root == Set({{2}, {3}}));
}

void TestApplyRule() {
  assert(ApplyRule(ProofNode(Set({Formula(1)})), RNot).empty());

  vector<ProofNode> ns =
      ApplyRule(ProofNode(Set({Formula(op_not, {{op_not, {3}}})})), RNot);
  assert(ns.size() == 1);
  assert(ns[0].root == Set({Formula(3)}));

  ns = ApplyRule(
      ProofNode(Set({Formula(1), Formula(op_impl, {1, {op_equiv, {{1}, {2}}}}),
                     Formula(op_not, {{1}})})),
      RImpl);
  assert(ns.size() == 2);
  assert(ns[0].root == Set({Formula(1), Formula(op_not, {{1}})}));
  assert(ns[1].root == Set({Formula(1), Formula(op_not, {{1}}),
                            Formula(op_equiv, {{1}, {2}})}));
}

void TestSimple() {
  ProofNode node(Set({}));
  assert(node.subnodes.empty());
}

int main() {
  TestBuildChildNodes();
  TestApplyRule();
  TestSimple();
}