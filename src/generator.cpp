#include "generator.h"

int GetRandom(int start, int end) {
  assert(end > start);
  assert(start >= 0);
  return (rand() % (end - start)) + start;
}
int GetRandom(int end) { return GetRandom(0, end); }

Formula GetRandomFormula(int subformulas, int variables_size,
                         const vector<Formula> &interesting) {
  assert(subformulas > 0);
  if ((subformulas == 2 || subformulas == 3) && ((rand() % 100) == 0)) {
    return interesting[rand() % interesting.size()];
  }
  if (subformulas == 1) {
    // return Formula(string(1, 'a' + (rand() % variables_size)));
    int r = rand() % 20;
    if (r == 0) {
      return FId(Formula(0), FNot(Formula(0)));
    } else if (r == 1) {
      return FId(FNot(Formula(0)), Formula(0));
    } else if (r == 2) {
      return FNot(FId(Formula(0), FNot(Formula(0))));
    } else if (r == 3) {
      return FNot(FId(FNot(Formula(0)), Formula(0)));
    } else if (r % 2) {
      return Formula(0);
    } else {
      return FNot(Formula(0));
    }
  }

  Operator op = static_cast<Operator>(GetRandom(3));
  if (op == op_not) {
    return FNot(GetRandomFormula(subformulas - 1, variables_size, interesting));
  } else {
    if (op == op_equiv && subformulas > 2)
      subformulas--;
    int left = GetRandom(1, subformulas);
    int right = subformulas - left;
    return Formula(op, {GetRandomFormula(left, variables_size, interesting),
                        GetRandomFormula(right, variables_size, interesting)});
  }
}