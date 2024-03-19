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