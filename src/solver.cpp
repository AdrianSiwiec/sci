#include "solver.h"
#include "commons.h"
#include "preprocessing.h"
#include <chrono>
#include <ctime>
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
  if (f.IsOp(op_not) && f.Subformula() == g)
    return true;
  if (g.IsOp(op_not) && g.Subformula() == f)
    return true;
  return false;
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
                                  const vector<Set> &formulas_to_add) {

  // Would still work but return empty. I think it's not needed.
  assert(!formulas_to_add.empty());

  vector<ProofNode> to_return;
  for (const Set &formulas : formulas_to_add) {
    Set s = node.root;
    if (!node.phase2) {
      if (formulas.Formulas().size() == 1) {
        s.ReplaceFormula(formula_to_remove, formulas.Formulas()[0]);
      } else {
        s.RemoveFormula(formula_to_remove, false);
        for (const Formula &formula : formulas.Formulas()) {
          s.AddFormula(formula, false);
        }
        s.Normalize();
      }
    } else {
      for (const Formula &formula : formulas.Formulas()) {
        s.AddFormula(formula, false);
      }
      s.Normalize();
    }
    to_return.push_back(ProofNode(std::move(s)));
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
    // set<Formula> previous_formulas_set;
    // for (const auto &f : previous_node.root.Formulas()) {
    //   previous_formulas_set.insert(f);
    // }

    bool changed = false;
    for (const auto &pf : conclusions) {
      for (const auto &formula : pf.root.Formulas()) {
        // if (previous_formulas_set.count(formula) == 0) {
        if (!Contains(previous_node.root.Formulas(), formula)) {
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
                            set<pair<int, FormulaSet>> &applied_rules,
                            const ProofNode &previous) {

  for (int i = 0; i < node.root.Formulas().size(); i++) {
    if (IsSingleUseRule(rule) &&
        WasRuleApplied(rule, node.root.Formulas()[i], applied_rules)) {
      continue;
    }
    vector<Set> result;
    if (rule != nullptr) {
      result = rule(node.root.Formulas()[i]);
    } else {
      result =
          RFun(node.root.Formulas()[i], node.root.Formulas(), !node.phase2);
    }
    if (!result.empty()) {
      vector<ProofNode> to_return;
      if (rule != nullptr) {
        to_return = BuildChildNodes(node, i, result);
      } else {
        ProofNode pn(result[0]);
        to_return.push_back(pn);
      }

      // If what we produced is what we already have, don't apply the rule
      if (to_return.size() == 1 && to_return[0].root == node.root) {
        continue;
      }

      // if (NothingChanged(previous, to_return)) {
      //   // cout << "Nothing changed" << endl;
      //   continue;
      // }

      if (IsSingleUseRule(rule))
        MarkRuleAsApplied(rule, node.root.Formulas()[i], applied_rules);
      node.formula_used = node.root.Formulas()[i];
      node.rule_used = GetRuleName(rule);
      return to_return;
    }
  }
  return {};
}

bool IsClosed(ProofNode &n) {
  Solve(n);
  assert(n.is_closed.has_value());
  return n.is_closed.value();
}

// std::chrono::_V2::system_clock::time_point start;

void Solve(ProofNode &n, ProofNode previous,
           set<pair<int, FormulaSet>> applied_rules) {
  // if (previous.subnodes.empty()) {
  // start = std::chrono::system_clock::now();
  // }

  // auto end = std::chrono::system_clock::now();
  // std::chrono::duration<double> elapsed_seconds = end - start;
  // if (elapsed_seconds.count() > 60) {
  // cout << "Finishing on " << n.root << endl;
  // exit(0);
  // }

  // cout << "Solving: " << n.root << endl;
  // cout << "\tPrevious: " << endl;
  // PrintProofNode(previous, "\t\t");

  if (IsClosed(n.root)) {
    n.is_closed = true;
    return;
  }

  // If subnodes are not empty, it was already solved
  if (n.subnodes.empty()) {
    if (!n.phase2) {
      bool usedPhase1 = false;
      for (auto rule : Phase1Rules) {
        // Modifies applied_rules
        n.subnodes = ApplyRule(n, rule, applied_rules, previous);
        if (!n.subnodes.empty()) {
          usedPhase1 = true;
          break;
        }
      }
      if (!usedPhase1)
        n.phase2 = true;
    }
    if (n.phase2) {
      for (auto rule : Phase2Rules) {
        n.subnodes = ApplyRule(n, rule, applied_rules, previous);
        if (!n.subnodes.empty()) {
          break;
        }
      }
    }
  }

  if (n.subnodes.empty()) {
    n.is_closed = false;
    return;
  }

  for (auto &subnode : n.subnodes) {
    subnode.phase2 = n.phase2;
    Solve(subnode, n, applied_rules);
    assert(subnode.is_closed.has_value());
    if (!subnode.is_closed.value()) {
      n.is_closed = false;
      return;
    }
  }
  n.is_closed = true;
}

vector<Formula> DoParseFormulas(string input_string, bool print) {
  vector<string> input_formulas = SplitString(input_string, ",");
  if (print)
    cout << "Your input: \"" << input_string
         << "\" parsed as formulas:" << endl;
  vector<Formula> formulas;
  for (const auto &s : input_formulas) {
    auto f = ParseInputFormula(s);
    if (f.has_value()) {
      if (print)
        cout << "\t" << f.value();
      Formula post = PostprocessFormula(f.value());
      if (print && f.value() != post)
        cout << "\twhich is translated as: " << post;
      if (print)
        cout << endl;
      formulas.push_back(post);
    } else {
      if (print)
        cout << "\tThis formula didn't parse: " << s << endl;
    }
  }
  return formulas;
}

ProofNode DoSolve(vector<Formula> formulas, bool print) {
  if (formulas.empty()) {
    return ProofNode(Set(vector<Formula>()));
  }

  if (print)
    cout << "\nWhich produces the following proof tree:" << endl;

  Set s(formulas);
  ProofNode n(s);
  Solve(n, ProofNode(Set(vector<Formula>())), {});
  if (print)
    PrintProofNode(n);
  if (print)
    cout << endl;

  if (print) {
    cout << "The total size of the (calculated) proof tree is: " << GetSize(n)
         << endl;
    cout << "The maximum depth of the (calculated) proof tree is: "
         << GetDepth(n) << endl;
  }
  return n;
}
ProofNode DoSolve(string input_string, bool print) {
  ClearVars();

  auto formulas = DoParseFormulas(input_string, print);
  return DoSolve(formulas, print);
}