#include "rules.h"
#include "solver.h"

void TestBuildChildNodes() {
  ProofNode n(Set(vector<Formula>({Formula(1), Formula(2), Formula(3)})));

  vector<ProofNode> children = BuildChildNodes(
      n, 0, {Set(vector<Formula>({{9}, {2}, {5}})), Set({Formula(3)})});

  assert(children.size() == 2);
  assert(children[0].root == Set(vector<Formula>({{2}, {3}})));
  assert(children[1].root == Set(vector<Formula>({{2}, {3}, {5}, {9}})));
}

void TestApplyRule() {
  ProofNode pn(Set({Formula(1)}));
  set<pair<int, FormulaSet>> ar;
  assert(ApplyRule(pn, RNot, ar).empty());

  Formula nn3(op_not, {{op_not, {3}}});

  pn = ProofNode(Set({nn3}));
  vector<ProofNode> ns = ApplyRule(pn, RNot, ar);
  assert(ns.size() == 1);
  assert(ns[0].root == Set({Formula(3)}));

  Formula eq12(op_id, {{1}, {2}});
  Formula n1(op_not, {{1}});
  pn = ProofNode(Set({Formula(1), Formula(op_impl, {1, eq12}), n1}));
  ns = ApplyRule(pn, RImpl, ar);
  assert(ns.size() == 2);
  assert(ns[0].root == Set({Formula(1), n1}));
  assert(ns[1].root == Set({Formula(1), n1, eq12}));

  // TODO: test if we don't return anything if nothing changed, when there is a
  // rule that can produce no change.
}

void TestAx() {
  assert(!IsAx1(Formula(op_not, {{op_id, {{1}, {2}}}})));
  assert(IsAx1(Formula(op_not, {{op_id, {{1}, {1}}}})));

  assert(!IsAx2(Formula(4), Formula(4)));
  assert(!IsAx2(Formula(4), Formula(op_not, {{5}})));
  assert(IsAx2(Formula(4), Formula(op_not, {{4}})));
  assert(!IsAx2(Formula(op_not, {{4}}), Formula(op_not, {{4}})));
  assert(IsAx2(Formula(op_not, {{4}}), Formula(4)));

  assert(IsClosed(Set("p,q,-p,-q,(p=q)")));
}

void TestSimple() {
  ProofNode n(Set(vector<Formula>{3}));
  assert(!IsClosed(n));
  assert(n.subnodes.empty());

  n.root.AddFormula(Formula(op_not, {{op_id, {{2}, {2}}}}));
  assert(IsClosed(n));
  assert(n.subnodes.empty());

  n.root.ReplaceFormula(
      1, Formula(op_not, {{op_not, {{op_not, {{op_id, {{3}, {3}}}}}}}}));
  assert(IsClosed(n));
  assert(n.subnodes.size() == 1);
  assert(n.subnodes[0].root.Formulas().size() == 2);
  assert(n.subnodes[0].root.Formulas()[0] == Formula(3));
  assert(n.subnodes[0].root.Formulas()[1] ==
         Formula(op_not, {{op_id, {{3}, {3}}}}));
}

void TestExamples() {
  ProofNode pn = DoSolve("p,q, -(p=q)", false);
  assert(pn.subnodes[1] == Set("p,q,-p,-(p=q)"));
  assert(pn.subnodes[1].is_closed == nullopt);
  assert(pn.subnodes[0].is_closed.value() == false);

  pn = DoSolve(
      "v12, v10, v9, v6, v4, v3, r, q, -p, -v8, -v11, (p=p), (v4=v3), "
      "(v8=-v9), (v11=-v10), (v11=-v12), (v6=(p->r)), (v12=(p->p)), "
      "(v7=(p->v8)), (v5=(p=q)), (v4=(v5->v6)), (v4=(v7=p)), (v9=(v10->v11))",
      false);
  assert(pn.is_closed.value());

  // v5=(q=p) is false, because: q, -p
  // v3=(v5->v6) is true, because v5 is false
  // -v3, v3
  pn = DoSolve(
      "q, ¬p, ¬v3, ¬v4,(v5≡(q≡p)), v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))",
      false);
  assert(pn.is_closed.value());

  pn =
      DoSolve("q, v8, ¬p, ¬v3, ¬v4, ¬v9, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), "
              "(v12≡(p→p)), (v7≡(p→v8)), (v6≡(p→r)), (v5≡(q≡p)), (v3≡(p≡v7)), "
              "(v3≡(v5→v6)), (v9≡(v10→v11))",
              false);
  assert(pn.is_closed.value());

  pn = DoSolve(
      "q, ¬p, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), (v6≡(p→r)), (v12≡(p→p)), "
      "(v7≡(p→v8)), (v5≡(p≡q)), (v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))",
      false);
  assert(pn.is_closed.value());

  pn = DoSolve("-p, q, (p≡p), (v3≡v4), (v4≡(p≡(p→¬((p→p)→¬(p→p))))), "
               "(v3≡((p≡q)→(p→r)))",
               false);
  assert(pn.is_closed.value());

  pn = DoSolve("¬p, q, ((¬(r→¬p)→(p≡p))→¬((p≡p)→¬(r→¬p))), "
               "((p≡(p→¬((p→p)→¬(p→p))))≡((q≡p)→(p→r)))",
               false);
  assert(pn.is_closed.value());

  pn = DoSolve(
      "-((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <>"
      "(p = p)) | ((p & p) | -q)))",
      false);
  assert(pn.is_closed.value());

  pn = DoSolve("¬b, ¬(b→b)", false);
  assert(pn.is_closed.value());

  pn = DoSolve("¬(((b→b)→b)→b)", false);
  assert(pn.is_closed.value());

  assert(DoSolve("b≡¬¬¬b", false).is_closed.value());

  assert(DoSolve("b≡((b↔b)∧¬b)", false).is_closed.value());
  assert(DoSolve("a≡(a→¬a)", false).is_closed.value());
  assert(DoSolve("a≡¬¬(a→¬a)", false).is_closed.value());
  // Is it really closed?
  assert(DoSolve("¬(((a∧b)≡(b∧b))→(b→a))", true).is_closed.value());
  assert(DoSolve("(b≡((b→(b→b))→(b→¬b)))", false).is_closed.value());
  assert(DoSolve("(c≡(c→¬((b→c)→(c→c))))", false).is_closed.value());
  assert(DoSolve("(¬(a→a)≡(b→(a→(a→b))))", false).is_closed.value());
  assert(DoSolve("(¬(a≡a)≡(a→(b→(b→b))))", false).is_closed.value());
  assert(DoSolve("((a≡¬a)≡((b→a)→(a→a)))", false).is_closed.value());
  assert(DoSolve("¬((¬¬a≡(a→(b→¬b)))→¬b)", false).is_closed.value());
  assert(DoSolve("a≡¬((b↔a)↔b)", false).is_closed.value());
  assert(DoSolve("b≡((b↔b)↔¬b)", false).is_closed.value());
  assert(DoSolve("¬¬((b↔a)≡(b↔¬a))", false).is_closed.value());
  assert(DoSolve("¬¬((b↔a)≡(a↔¬b))", false).is_closed.value());
  assert(DoSolve("¬(b↔b)≡((a∧a)↔a)", false).is_closed.value());
}

int main() {
  TestBuildChildNodes();
  TestApplyRule();
  TestAx();
  TestSimple();
  TestExamples();
}