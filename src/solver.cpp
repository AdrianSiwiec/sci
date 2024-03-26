#include "solver.h"

vector<ProofNode> BuildChildNodes(const ProofNode &node, int formula_to_remove,
                                  vector<Set> formulas_to_add) {

  // Would still work but return empty. I think it's not needed.
  assert(!formulas_to_add.empty());

  vector<ProofNode> to_return;
  for (const Set &formulas : formulas_to_add) {
    Set s = node.root;
    s.RemoveFormula(formula_to_remove);
    for (const Formula &formula : formulas.Formulas()) {
      s.AddFormula(formula);
    }
    to_return.push_back(ProofNode(s));
  }
  return to_return;
}
vector<ProofNode> ApplyRule(const ProofNode node, Rule rule) {
  // TODO: fun will be when rule == nullptr
  for (int i = 0; i < node.root.Formulas().size(); i++) {
    vector<Set> result = rule(node.root.Formulas()[i]);
    if (!result.empty()) {
      auto to_return = BuildChildNodes(node, i, result);

      // If what we produced is what we already have, don't apply the rule
      if (to_return.size() == 1 && to_return[0].root == node.root) {
        continue;
      }
      return to_return;
    }
  }
  return {};
}

bool IsAx1(const Formula &f) {
  if (!f.IsOp(op_not))
    return false;

  const auto &f1 = f.Subformula(0);
  if (!f1.IsOp(op_equiv))
    return false;

  return f1.Subformula(0) == f1.Subformula(1);
}

bool IsAx2(const Formula &f, const Formula &g) {
  return (f == Formula(op_not, {g})) || (g == Formula(op_not, {f}));
}

bool IsClosed(const Set &set) {
  for (const auto &f : set.Formulas()) {
    if (IsAx1(f))
      return true;
    for (const auto &g : set.Formulas()) {
      if (IsAx2(f, g))
        return true;
    }
  }
  return false;
}

bool IsClosed(ProofNode &n) {
  if (IsClosed(n.root))
    return true;

  // If subnodes are not empty, it was already solved
  if (n.subnodes.empty()) {
    for (auto rule : AllRules) {
      n.subnodes = ApplyRule(n, rule);
      if (!n.subnodes.empty())
        break;
    }
  }

  if (n.subnodes.empty())
    return false;
  for (auto &subnode : n.subnodes) {
    if (!IsClosed(subnode))
      return false;
  }
  return true;
}