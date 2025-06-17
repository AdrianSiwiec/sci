#include "label_solver.h"
#include "preprocessing.h"
#include "stats.h"

vector<LabelNode> ApplyRule(const LabelNode &n, LabelRule rule,
                            bool is_positive, int label) {
  if (n.used_rules.count(make_pair(label, rule)) == 0) {
    auto result = rule(n, label);
    if (!result.empty()) {
      for (auto &node : result) {
        node.used_rules.insert(make_pair(label, rule));
      }
      return result;
    }
  }
  return {};
}

vector<LabelNode> ApplyRule(LabelNode &n, LabelRule rule, bool is_positive) {
  if (is_positive) {
    for (int i = 1; i < n.root.max_label; i++) {
      auto result = ApplyRule(n, rule, is_positive, i);
      if (!result.empty()) {
        n.rule_used = GetLabelRuleString(rule);
        n.label_used = i;
        return result;
      }
    }
  } else {
    for (int i = -1; i > n.root.min_label; i--) {
      auto result = ApplyRule(n, rule, is_positive, i);
      if (!result.empty()) {
        n.rule_used = GetLabelRuleString(rule);
        n.label_used = i;
        return result;
      }
    }
  }
  return {};
}

bool ApplyEqRule(LabelNode &n, LabelEqRule rule) {
  bool result = rule(n);
  if (result) {
    n.eq_rules_used.push_back(GelLabelEqRuleString(rule));
  }
  return result;
}

void LabelSolve(LabelNode &n) {
  while (true) {
    if (ApplyEqRule(n, LERF))
      continue;
    if (ApplyEqRule(n, LERNot))
      continue;
    if (ApplyEqRule(n, LERImpl))
      continue;
    if (ApplyEqRule(n, LEREq))
      continue;

    break;
  }

  // ----------------- Termination Rules -------------
  // Termination rule 1
  for (const auto &ne : n.root.GetNotEquals()) {
    if (n.root.IsEqual(ne.first, ne.second)) {
      n.is_closed = true;
      return;
    }
  }
  // Termination rule 2
  for (int p = 1; p < n.root.max_label; p++) {
    for (int neg = -1; neg > n.root.min_label; neg--) {
      if (n.root.IsEqual(p, neg)) {
        n.is_closed = true;
        return;
      }
    }
  }

  // ----------------- Expansion Rules -------------
  vector<LabelNode> subnodes;
  subnodes = ApplyRule(n, LRNotPlus, true);
  if (subnodes.empty())
    subnodes = ApplyRule(n, LRNotMinus, false);
  if (subnodes.empty())
    subnodes = ApplyRule(n, LRImplMinus, false);
  if (subnodes.empty())
    subnodes = ApplyRule(n, LRIdPlus, true);
  if (subnodes.empty())
    subnodes = ApplyRule(n, LRImplPlus, true);
  if (subnodes.empty())
    subnodes = ApplyRule(n, LRIdMinus, false);

  if (!subnodes.empty()) {
    n.subnodes = subnodes;
    for (auto &subnode : n.subnodes) {
      LabelSolve(subnode);
      assert(subnode.is_closed.has_value());
      if (subnode.is_closed.value() == false) {
        n.is_closed = false;
        return;
      }
    }

    n.is_closed = true;
  } else {
    n.is_closed = false;
  }
}

LabelNode DoSolveLabel(const vector<Formula> &formulas, StatsAtom &statsAtom,
                       bool print) {
  if (formulas.empty())
    return LabelNode();

  if (print)
    cout << "\nWhich produces the following proof tree:" << endl;
  LabelNode ln;
  for (const auto &f : formulas) {
    ln.root.CreateLabel(true, f);
  }
  {
    DurationMeasurer dm(statsAtom.duration);
    LabelSolve(ln);
  }
  if (print) {
    PrintLabelNode(ln);
    cout << endl;
  }

  statsAtom.treeSize = GetSize(ln);
  statsAtom.treeDepth = GetDepth(ln);

  return ln;
}

LabelNode DoSolveLabel(string input_string, StatsAtom &statsAtom, bool print) {
  ClearVars();
  auto formulas = DoParseFormulas(input_string, print);
  return DoSolveLabel(formulas, statsAtom, print);
}

void TestInput(string input_string, bool print) {
  ClearVars();
  Stats stats;

  auto proof_node = DoSolve(input_string, stats.tStar, false);
  auto label_node = DoSolveLabel(input_string, stats.label, false);

  if (print && proof_node.is_closed.value() != label_node.is_closed.value()) {
    PrintProofNode(proof_node);
    PrintLabelNode(label_node);
  }

  cout << "tStar: " << stats.tStar.duration / 1ms
       << "ms, size=" << stats.tStar.treeSize
       << ", depth=" << stats.tStar.treeDepth << endl
       << "label: " << stats.label.duration / 1ms
       << "ms, size=" << stats.label.treeSize
       << ", depth=" << stats.label.treeDepth << endl
       << endl;

  assert(proof_node.is_closed.value() == label_node.is_closed.value());
}

bool TestFormula(const Formula &f, bool print) {
  vector<Formula> vec{PostprocessFormula(f)};

  Stats stats;

  auto proof_node = DoSolve(vec, stats.tStar, false);
  auto label_node = DoSolveLabel(vec, stats.label, false);

  if (print && proof_node.is_closed.value() != label_node.is_closed.value()) {
    PrintProofNode(proof_node);
    PrintLabelNode(label_node);
    cout << "a = " << Formula("a").Var() << endl;
    cout << "b = " << Formula("b").Var() << endl;
  }

  assert(proof_node.is_closed.value() == label_node.is_closed.value());

  return proof_node.is_closed.value();
}