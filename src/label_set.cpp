#include "label_set.h"

int LabelSet::CreateLabel(bool positive, Formula f) {
  // TODO: find union stuff
  if (positive) {
    assert(labels.count(max_label) == 0);
    labels.insert(make_pair(max_label, f));
    parent[max_label] = max_label;
    size[max_label] = 1;
    return max_label++;
  } else {
    assert(labels.count(min_label) == 0);
    labels.insert(make_pair(min_label, f));
    parent[min_label] = min_label;
    size[min_label] = 1;
    return min_label--;
  }
}

const Formula &LabelSet::GetFormula(int label) {
  assert(labels.count(label) > 0);
  return labels.at(label);
}

int LabelSet::Find(int label) {
  if (parent[label] != label) {
    int answer = Find(parent[label]);
    parent[label] = answer;
  }
  return parent[label];
}

void LabelSet::Union(int a, int b) {
  if (Find(a) == Find(b))
    return;
  if (size[parent[a]] < size[parent[b]]) {
    swap(a, b);
  }
  size[parent[a]] += size[parent[b]];
  parent[parent[b]] = parent[a];
}

bool LabelSet::IsEqual(int a, int b) {
  assert(a <= max_label && a >= min_label && b <= max_label && b >= min_label &&
         a != 0 && b != 0);
  return Find(a) == Find(b);
}
void LabelSet::MakeEqual(int a, int b) {
  assert(a <= max_label && a >= min_label && b <= max_label && b >= min_label &&
         a != 0 && b != 0);
  Union(Find(a), Find(b));
}