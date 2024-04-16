#include <map>
#include <set>

#include "commons.h"
#include "structs.h"

// ints represents labels. If the label (int) is positive, it is a positive
// label. If it is negative, it is a negative label.
struct LabelSet {
  // Returns the created label
  int CreateLabel(bool positive, Formula f);
  const Formula &GetFormula(int label);
  const map<int, Formula> &GetLabels() { return labels; }
  bool IsEqual(int a, int b);
  void MakeEqual(int a, int b);
  set<pair<int, int>> not_equal;

private:
  map<int, Formula> labels;
  int max_label = 1;
  int min_label = -1;

  // For find-union
  map<int, int> parent;
  map<int, int> size;
  int Find(int label);
  void Union(int a, int b);
};
