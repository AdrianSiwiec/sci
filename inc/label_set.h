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
  bool IsNotEqual(int a, int b);
  void MakeNotEqual(int a, int b);
  const set<pair<int, int>> &GetNotEquals() { return not_equal; }
  int max_label = 1;
  int min_label = -1;

private:
  map<int, Formula> labels;
  set<pair<int, int>> not_equal;

  // For find-union
  map<int, int> parent;
  map<int, int> size;
  int Find(int label);
  void Union(int a, int b);

  friend ostream &operator<<(ostream &os, LabelSet &s);
};

ostream &operator<<(ostream &os, const LabelSet &s);

struct LabelNode {
  LabelSet root;
  vector<LabelNode> subnodes;
  set<pair<int, vector<LabelNode> (*)(const LabelNode &n, int label)>>
      used_rules;

  optional<bool> is_closed = nullopt;
};

void PrintLabelNode(LabelNode &n, string prefix = "");