#include "solver.h"
#include "preprocessing.h"
#include <set>

bool IsAx1(const Formula &f) {
  if (!f.IsOp(op_not))
    return false;

  const auto &f1 = f.Subformula(0);
  if (!f1.IsOp(op_id))
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
  sort(to_return.begin(), to_return.end());
  auto last = unique(to_return.begin(), to_return.end());
  to_return.erase(last, to_return.end());
  return to_return;
}

bool NothingChanged(const ProofNode &previous,
                    const vector<ProofNode> &conclusions) {
  if (previous.subnodes.empty())
    return false;
  for (const auto &previous_node : previous.subnodes) {
    set<Formula> previous_formulas_set;
    for (const auto &f : previous_node.root.Formulas()) {
      previous_formulas_set.insert(f);
    }

    bool changed = false;
    for (const auto &pf : conclusions) {
      for (const auto &formula : pf.root.Formulas()) {
        if (previous_formulas_set.count(formula) == 0) {
          // We found new formula
          changed = true;
        }
      }
    }
    if (!changed)
      return true;
  }
  return false;
}

vector<ProofNode> ApplyRule(ProofNode &node, Rule rule,
                            const ProofNode &previous) {

  for (int i = 0; i < node.root.Formulas().size(); i++) {
    if (IsSingleUseRule(rule) &&
        WasRuleApplied(rule, node.root.Formulas()[i])) {
      continue;
    }
    vector<Set> result;
    if (rule != nullptr) {
      result = rule(node.root.Formulas()[i]);
    } else {
      result = RFun(node.root.Formulas()[i], node.root.Formulas());
    }
    if (!result.empty()) {
      auto to_return = BuildChildNodes(node, i, result);

      if (NothingChanged(previous, to_return)) {
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
      if (IsSingleUseRule(rule))
        MarkRuleAsApplied(rule, node.root.Formulas()[i]);
      node.formula_used = node.root.Formulas()[i];
      node.rule_used = GetRuleName(rule);
      return to_return;
    }
  }
  return {};
}

bool IsClosed(ProofNode &n) {
  Solve(n);
  return n.is_closed;
}

void Solve(ProofNode &n, ProofNode previous) {
  if (previous.subnodes.empty())
    ClearAppliedRules();

  // cout << "Solving: ";
  // PrintProofNode(n);
  // cout << "\tPrevious: " << endl;
  // PrintProofNode(previous, "\t\t");

  if (IsClosed(n.root)) {
    n.is_closed = true;
    return;
  }

  // If subnodes are not empty, it was already solved
  if (n.subnodes.empty()) {
    for (auto rule : AllRules) {
      n.subnodes = ApplyRule(n, rule, previous);
      if (!n.subnodes.empty())
        break;
    }
  }

  if (n.subnodes.empty()) {
    n.is_closed = false;
    return;
  }

  for (auto &subnode : n.subnodes) {
    Solve(subnode, n);
    if (!subnode.is_closed) {
      n.is_closed = false;
      return;
    }
  }
  n.is_closed = true;
}

void DoSolve(string input_string) {
  vector<string> input_formulas = SplitString(input_string, ",");
  cout << "Your input: " << input_string << " parsed as formulas:" << endl;
  vector<Formula> formulas;
  for (const auto &s : input_formulas) {
    auto f = ParseInputFormula(s);
    if (f.has_value()) {
      cout << "\t" << f.value();
      Formula post = PostprocessFormula(f.value());
      if (f.value() != post)
        cout << "\twhich is translated as: " << post;
      cout << endl;
      formulas.push_back(post);
    } else {
      cout << "\tThis formula didn't parse: " << s << endl;
    }
  }
  if (formulas.empty()) {
    return;
  }

  cout << "Which produces the following proof tree:" << endl;

  Set s(formulas);
  ProofNode n(s);
  IsClosed(n);
  PrintProofNode(n);
  cout << endl;
}