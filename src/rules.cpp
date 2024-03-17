#include "commons.h"
#include "structs.h"

bool matchesRNot(const Formula &f) {
  if (!f.IsOp(op_not) || f.Subformulas().empty())
    return false;
  return f.Subformula(0).IsOp(op_not);
}
vector<Set> RNot(const Formula &in) {
  if (matchesRNot(in)) {
    return {Set({in.Subformula(0).Subformula(0)})};
  }
  return {};
}

vector<Set> RImpl(const Formula &in) {
  if (in.IsOp(op_impl)) {
    vector<Set> result;
    Formula f = in;
    assert(f.Subformulas().size() == 2);
    result.push_back(Set({Formula(op_not, {f.Subformula(0)})}));
    result.push_back(Set({f.Subformula(1)}));
    return result;
  }
  return {};
}