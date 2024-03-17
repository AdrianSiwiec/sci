#include "commons.h"
#include "structs.h"

bool matchesRNot(const Formula &f) {
  if (!f.IsOp(op_not) || f.Subformulas().empty())
    return false;
  return f.Subformula(0).IsOp(op_not);
}

vector<Set> RNot(const Set &in) {
  for (int i = 0; i < in.formulas.size(); i++) {
    if (matchesRNot(in.formulas[i])) {
      Set result = in;
      result.formulas[i] = in.formulas[i].Subformula(0).Subformula(0);
      // TODO: normalize?
      return {result};
    }
  }
  return {};
}