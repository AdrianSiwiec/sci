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

const Formula &LabelSet::GetFormula(int label) const {
  assert(labels.count(label) > 0);
  return labels.at(label);
}

bool LabelSet::IsNotEqual(int a, int b) {
  if (a < b)
    swap(a, b);
  return not_equal.count(make_pair(a, b));
}
void LabelSet::MakeNotEqual(int a, int b) {
  if (a < b)
    swap(a, b);
  not_equal.insert(make_pair(a, b));
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

ostream &operator<<(ostream &os, LabelSet &s) {
  os << "Labels: {";
  for (int i = 1; i < s.max_label; i++) {
    os << i << ":" << s.GetFormula(i);
    if (s.min_label < -1 || i + 1 < s.max_label)
      os << ", ";
  }
  for (int i = -1; i > s.min_label; i--) {
    os << i << ":" << s.GetFormula(i);
    if (i - 1 > s.min_label)
      os << ", ";
  }
  os << "}";

  os << "\tEqualities:";
  set<int> printed;
  for (int i = s.min_label + 1; i < s.max_label; i++) {
    if (i == 0)
      continue;
    if (printed.count(i) > 0)
      continue;
    for (int j = i + 1; j < s.max_label; j++) {
      if (j == 0)
        continue;
      if (s.IsEqual(i, j)) {
        if (printed.count(i) == 0) {
          if (!printed.empty())
            os << ",";
          os << " " << i;
          printed.insert(i);
        }
        os << "=" << j;
        printed.insert(j);
      }
    }
  }

  os << ";\tNon-equalities:";
  bool printed_ne = false;
  for (const auto &ne : s.GetNotEquals()) {
    if (printed_ne)
      os << ",";
    printed_ne = true;
    os << " ";
    os << ne.second << "≠" << ne.first;
  }
  os<<";";

  return os;
}

void PrintLabelNode(LabelNode &n, string prefix) {
  cout << prefix << "--: " << n.root;
  if (n.label_used.has_value()) {
    cout << "\tApplied rule " << n.rule_used.value() << " to "
         << n.label_used.value() << " to generate descendant"
         << (n.subnodes.size() > 1 ? "s" : "")<<";";
  }
  cout << "\t(is ";
  if (n.is_closed.has_value()) {
    cout << (n.is_closed.value() ? "closed" : "open");
  } else {
    cout << "?";
  }
  cout << ")";
  cout << endl;
  for (int i = 0; i < n.subnodes.size(); i++) {
    string add_to_prefix = " |";
    PrintLabelNode(n.subnodes[i], prefix + add_to_prefix);
  }
}