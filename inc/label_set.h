#pragma once

#include <map>
#include <set>

#include "commons.h"
#include "structs.h"

// ints represents labels. If the label (int) is positive, it is a positive
// label. If it is negative, it is a negative label.
struct LabelSet {
  // Returns the created label
  int CreateLabel(bool positive, Formula f);
  const Formula &GetFormula(int label) const;
  const map<int, Formula> &GetLabels() { return labels; }
  bool IsEqual(int a, int b);
  void MakeEqual(int a, int b);
  set<pair<int, int>> not_equal;
  int max_label = 1;
  int min_label = -1;

private:
  map<int, Formula> labels;

  // For find-union
  map<int, int> parent;
  map<int, int> size;
  int Find(int label);
  void Union(int a, int b);
};

struct LabelNode {
  LabelSet root;
  vector<LabelNode> subnodes;
  set<pair<int, vector<LabelNode>(*)(const LabelNode &n, int label)>> used_rules;

  optional<bool> is_closed = nullopt;
};