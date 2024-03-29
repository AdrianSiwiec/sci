#include "solver.h"
#include <set>

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

bool NothingChanged(const vector<Formula> previous_formulas,
                    const vector<ProofNode> &conclusions) {
  set<Formula> previous_formulas_set;
  for (const auto &f : previous_formulas) {
    previous_formulas_set.insert(f);
  }

  // cout << "Running nothing changed: ";
  // for (const auto &f : previous_formulas) {
  //   cout << f << ", ";
  // }
  // cout << endl;

  for (const auto &pf : conclusions) {
    for (const auto &formula : pf.root.Formulas()) {
      if (previous_formulas_set.count(formula) == 0) {
        // cout << "\t" << formula << " is new!" << endl;
        // We found new formula
        return false;
      }
    }
  }
  return true;
}

vector<ProofNode> ApplyRule(const ProofNode node, Rule rule,
                            vector<Formula> previous_formulas) {
  // if (rule == RTerEq) {
  //   cout << "Start RTerEq" << endl;
  // }
  for (int i = 0; i < node.root.Formulas().size(); i++) {
    vector<Set> result;
    if (rule != nullptr) {
      result = rule(node.root.Formulas()[i]);
    } else {
      result = RFun(node.root.Formulas()[i], node.root.Formulas());
    }
    if (!result.empty()) {
      auto to_return = BuildChildNodes(node, i, result);

      if (NothingChanged(previous_formulas, to_return)) {
        continue;
      }

      // if (true || rule == RTerEq) {
      //   cout << "We got result" << endl;
      //   cout << "start: " << node.root << endl;
      //   cout << "previous: ";
      //   for (const auto &i : previous_formulas)
      //     cout << i << ", ";
      //   cout << endl;
      //   cout << "got: " << endl;
      //   for (const auto &n : to_return) {
      //     cout << "\t" << n.root << endl;
      //   }
      // }

      // If what we produced is what we already have, don't apply the rule
      // TODO: this is wrong. Need to compare sets
      if (to_return.size() == 1 && to_return[0].root == node.root) {
        continue;
      }
      return to_return;
    }
  }
  return {};
}

bool IsClosed(ProofNode &n) {
  Solve(n);
  return n.is_closed;
}

void Solve(ProofNode &n, vector<Formula> previous_formulas) {
  if (IsClosed(n.root)) {
    n.is_closed = true;
    return;
  }

  // If subnodes are not empty, it was already solved
  if (n.subnodes.empty()) {
    for (auto rule : AllRules) {
      n.subnodes = ApplyRule(n, rule, previous_formulas);
      if (!n.subnodes.empty())
        break;
    }
  }

  if (n.subnodes.empty()) {
    n.is_closed = false;
    return;
  }

  vector<Formula> child_formulas;
  for (const auto &subnode : n.subnodes) {
    for (const auto &formula : subnode.root.Formulas())
      child_formulas.push_back(formula);
  }

  for (auto &subnode : n.subnodes) {
    Solve(subnode, child_formulas);
    if (!subnode.is_closed) {
      n.is_closed = false;
      return;
    }
  }
  n.is_closed = true;
}