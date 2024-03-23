#include "rules.h"
#include "solver.h"

void TestBuildChildNodes() {
  ProofNode n(Set(vector<Formula>({Formula(1), Formula(2), Formula(3)})));

  vector<ProofNode> children = BuildChildNodes(
      n, 0, {Set(vector<Formula>({{9}, {2}, {5}})), Set({Formula(3)})});

  assert(children[0].root == Set(vector<Formula>({{2}, {3}, {5}, {9}})));
  assert(children[1].root == Set(vector<Formula>({{2}, {3}})));
}

void TestApplyRule() {
  assert(ApplyRule(ProofNode(Set({Formula(1)})), RNot).empty());

  Formula nn3(op_not, {{op_not, {3}}});

  vector<ProofNode> ns = ApplyRule(ProofNode(Set({nn3})), RNot);
  assert(ns.size() == 1);
  assert(ns[0].root == Set({Formula(3)}));

  Formula eq12(op_equiv, {{1}, {2}});
  Formula n1(op_not, {{1}});
  ns = ApplyRule(ProofNode(Set({Formula(1), Formula(op_impl, {1, eq12}), n1})),
                 RImpl);
  assert(ns.size() == 2);
  assert(ns[0].root == Set({Formula(1), n1}));
  assert(ns[1].root == Set({Formula(1), n1, eq12}));

  // TODO: test if we don't return anything if nothing changed, when there is a
  // rule that can produce no change.
}

void TestAx() {
  assert(!IsAx1(Formula(op_not, {{op_equiv, {{1}, {2}}}})));
  assert(IsAx1(Formula(op_not, {{op_equiv, {{1}, {1}}}})));

  assert(!IsAx2(Formula(4), Formula(4)));
  assert(!IsAx2(Formula(4), Formula(op_not, {{5}})));
  assert(IsAx2(Formula(4), Formula(op_not, {{4}})));
  assert(!IsAx2(Formula(op_not, {{4}}), Formula(op_not, {{4}})));
  assert(IsAx2(Formula(op_not, {{4}}), Formula(4)));
}

void TestSimple() {
  ProofNode n(Set(vector<Formula>{3}));
  assert(!IsClosed(n));
  assert(n.subnodes.empty());

  n.root.AddFormula(Formula(op_not, {{op_equiv, {{2}, {2}}}}));
  assert(IsClosed(n));
  assert(n.subnodes.empty());

  n.root.ReplaceFormula(
      1, Formula(op_not, {{op_not, {{op_not, {{op_equiv, {{3}, {3}}}}}}}}));
  assert(IsClosed(n));
  assert(n.subnodes.size() == 1);
  assert(n.subnodes[0].root.Formulas().size() == 2);
  assert(n.subnodes[0].root.Formulas()[0] == Formula(3));
  assert(n.subnodes[0].root.Formulas()[1] ==
         Formula(op_not, {{op_equiv, {{3}, {3}}}}));
}

int main() {
  TestBuildChildNodes();
  TestApplyRule();
  TestAx();
  TestSimple();
}