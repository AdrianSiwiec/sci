#include "commons.h"
#include "structs.h"

bool matchesRNot(const Formula &f) {
  if (f.is_var || f.op != op_not || f.subformulas.empty())
    return false;
  return f.subformulas[0].op == op_not;
}

vector<Set> RNot(const Set &in) {
  for (int i = 0; i < in.formulas.size(); i++) {
    if (matchesRNot(in.formulas[i])) {
      Set result = in;
      result.formulas[i] = in.formulas[i].subformulas[0].subformulas[0];
      // TODO: normalize?
      return {result};
    }
  }
  return {};
}