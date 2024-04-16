#include "label_set.h"

void TestBasic() {
  LabelSet ls;
  Formula f("p");
  assert(ls.CreateLabel(true, f) == 1);
  assert(ls.CreateLabel(true, f) == 2);
  assert(ls.CreateLabel(false, f) == -1);
  assert(ls.CreateLabel(false, f) == -2);
  assert(ls.CreateLabel(false, f) == -3);
  assert(ls.CreateLabel(false, f) == -4);
  assert(ls.CreateLabel(true, f) == 3);

  assert(ls.GetFormula(-3) == f);
}

void TestEqual() {
  LabelSet ls;
  Formula f("p");
  for (int i = 1; i <= 10; i++) {
    assert(ls.CreateLabel(true, f) == i);
    assert(ls.CreateLabel(false, f) == -i);
  }
  for (int i = 1; i <= 10; i++) {
    for (int j = 1; j <= 10; j++) {
      assert(ls.IsEqual(i, j) == (i == j));
      assert(ls.IsEqual(-i, -j) == (i == j));
    }
  }

  ls.MakeEqual(1, 3);
  ls.MakeEqual(7, 9);
  ls.MakeEqual(5, 9);
  ls.MakeEqual(5, 3);

  ls.MakeEqual(-1, -9);
  ls.MakeEqual(-1, -3);
  ls.MakeEqual(-1, -5);
  ls.MakeEqual(-3, -7);
  for (int i = 1; i <= 10; i++) {
    for (int j = 1; j <= 10; j++) {
      // Equal when both are odd, or i=j
      assert(ls.IsEqual(i, j) == (((i * j) % 2) || (i == j)));
      assert(ls.IsEqual(-i, -j) == (((i * j) % 2) || (i == j)));
    }
  }
  ls.MakeEqual(3, -7);
  for (int i = 1; i <= 10; i++) {
    for (int j = 1; j <= 10; j++) {
      // Equal when both are odd
      assert(ls.IsEqual(i, -j) == (((i * j) % 2)));
      assert(ls.IsEqual(-i, j) == (((i * j) % 2)));
    }
  }
}

int main() {
  TestBasic();
  TestEqual();
}