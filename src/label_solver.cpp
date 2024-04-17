#include "label_solver.h"

vector<LabelNode> ApplyRule(const LabelNode &n, LabelRule rule,
                            bool is_positive) {
  if (is_positive) {
    for (int i = 1; i < n.root.max_label; i++) {
      if (n.used_rules.count(make_pair(i, rule)) == 0) {
        auto result = rule(n, i);
        if (!result.empty())
          return result;
      }
    }
  } else {
    for (int i = -1; i > n.root.min_label; i--) {
      if (n.used_rules.count(make_pair(i, rule)) == 0) {
        auto result = rule(n, i);
        if (!result.empty())
          return result;
      }
    }
  }
  return {};
}

void LabelSolve(LabelNode &n) {
  // TODO: expand equalities

  // Termination rule 1
  for (const auto &ne : n.root.not_equal) {
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

  vector<LabelNode> subnodes;
  ApplyRule(n, LRNotPlus, true);
  // if subnodes empty apply next rule

  if (!subnodes.empty()) {
    n.subnodes = subnodes;
    for (auto &subnode : n.subnodes) {
      LabelSolve(subnode);
    }

    n.is_closed = true;
    for (auto &subnode : n.subnodes) {
      if (subnode.is_closed.value() == false)
        n.is_closed = false;
    }
  } else {
    n.is_closed = false;
  }
}