#include "generator.h"

int GetRandom(int start, int end) {
  assert(end > start);
  assert(start >= 0);
  return (rand() % (end - start)) + start;
}
int GetRandom(int end) { return GetRandom(0, end); }

Formula GetRandomFormula(int subformulas, int variables_size) {
  assert(subformulas > 0);
  if (subformulas == 1)
    return Formula(string(1, 'p' + (rand() % variables_size)));

  if (subformulas == 2)
    return FNot(Formula(string(1, 'p' + (rand() % variables_size))));

  Operator op = static_cast<Operator>(GetRandom(3));
  if (op == op_not) {
    return FNot(GetRandomFormula(subformulas - 1, variables_size));
  } else {
    subformulas--;

    int left = GetRandom(1, subformulas);
    int right = subformulas - left;
    return Formula(op, {GetRandomFormula(left, variables_size),
                        GetRandomFormula(right, variables_size)});
  }
}