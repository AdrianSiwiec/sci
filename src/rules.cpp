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

bool matchesRNotImpl(const Formula &f) {
  return f.IsOp(op_not) && f.Subformula(0).IsOp(op_impl);
}
vector<Set> RNotImpl(const Formula &f) {
  if (matchesRNotImpl(f)) {
    return {Set({f.Subformula(0).Subformula(0),
                 Formula(op_not, {f.Subformula(0).Subformula(1)})})};
  }
  return {};
}

bool matchesRFun(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar();
}
Formula ReplaceAll(const Formula &f, const Formula &to_replace,
                   const Formula &replace_with) {
  if (f == to_replace) {
    return replace_with;
  }
  if (f.IsVar())
    return f;
  if (f.IsOp(op_not))
    return Formula(op_not,
                   {ReplaceAll(f.Subformula(), to_replace, replace_with)});
  return Formula(f.Op(),
                 {ReplaceAll(f.Subformula(0), to_replace, replace_with),
                  ReplaceAll(f.Subformula(1), to_replace, replace_with)});
}
vector<Set> RFun(const Formula &f, Set s) {
  if (matchesRFun(f)) {
    Formula to_replace = f.Subformula(1);
    Formula replace_with = f.Subformula(0);
    vector<Formula> to_return;
    auto formulas = s.Formulas();
    for (const Formula &formula : formulas) {
      if (f != formula)
        to_return.push_back(ReplaceAll(formula, to_replace, replace_with));
      else
        to_return.push_back(f);
    }
    return {Set(to_return)};
  }
  return {};
}