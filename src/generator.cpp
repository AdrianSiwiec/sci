#include "generator.h"

int GetRandom(int start, int end) {
  assert(end > start);
  assert(start >= 0);
  return (rand() % (end - start)) + start;
}
int GetRandom(int end) { return GetRandom(0, end); }

Formula GetRandomKRZFormula(int subformulas, int variables_size,
                            const vector<Formula> &vars) {
  assert(subformulas > 0);
  if (subformulas == 1) {
    return vars[rand() % variables_size];
  }
  if (rand() % 2) {
    return FNot(GetRandomKRZFormula(subformulas - 1, variables_size, vars));
  } else {
    int left = GetRandom(1, subformulas);
    int right = subformulas - left;
    return Formula(Operator::op_impl,
                   {GetRandomKRZFormula(left, variables_size, vars),
                    GetRandomKRZFormula(right, variables_size, vars)});
  }
}

Formula GetRandomFormula(int subformulas, int variables_size,
                         const vector<Formula> &interesting) {
  assert(subformulas > 0);
  if (!interesting.empty() && (subformulas == 2 || subformulas == 3) &&
      ((rand() % 100) == 0)) {
    return interesting[rand() % interesting.size()];
  }
  if (subformulas == 1) {
    int r = rand() % 8;
    if (r == 0) {
      static Formula f = Formula(0);
      return f;
    }
    if (r == 1) {
      static Formula f = Formula(1);
      return f;
    }
    if (r == 2) {
      static Formula f = Formula(2);
      return f;
    }
    if (r == 3) {
      static Formula f = FId(Formula(1), Formula(2));
      return f;
    }
    if (r == 4) {
      static Formula f = FId(Formula(0), Formula(1));
      return f;
    }
    if (r == 5) {
      static Formula f = FId(Formula(0), Formula(2));
      return f;
    }
    if (r == 6) {
      static Formula f =
          FImpl(FId(Formula(1), Formula(2)), FId(Formula(0), Formula(2)));
      return f;
    }
    if (r == 7) {
      static Formula f = FImpl(
          FId(Formula(0), Formula(1)),
          FImpl(FId(Formula(1), Formula(2)), FId(Formula(0), Formula(2))));
      return f;
    }

    // return Formula((rand() % variables_size));

    // int r = rand() % 10;
    // if (r == 0) {
    //   return FId(Formula(0), FNot(Formula(0)));
    // } else if (r == 1) {
    //   return FId(FNot(Formula(0)), Formula(0));
    // } else if (r == 2) {
    //   return FNot(FId(Formula(0), FNot(Formula(0))));
    // } else if (r == 3) {
    //   return FNot(FId(FNot(Formula(0)), Formula(0)));
    // } else if (r % 2) {
    //   return Formula(0);
    // } else {
    //   return FNot(Formula(0));
    // }
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