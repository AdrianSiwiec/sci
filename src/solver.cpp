#include "solver.h"

vector<ProofNode> BuildChildNodes(const ProofNode &node, int formula_to_remove,
                                  vector<Set> formulas_to_add) {
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
vector<ProofNode> ApplyRule(const ProofNode &node, Rule rule) {
  // TODO: fun will be when rule == nullptr
  for (int i = 0; i < node.root.Formulas().size(); i++) {
    vector<Set> result = rule(node.root.Formulas()[i]);
    if (!result.empty()) {
      // TODO: check if what we got is not the same as what we received
      return BuildChildNodes(node, i, result);
    }
  }
  return {};
}

void PopulateSubnodes(ProofNode &node) { assert(node.subnodes.empty()); }