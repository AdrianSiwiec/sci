#include "commons.h"
#include "preprocessing.h"
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

int GetNewVar() {
  int var = variable_to_int.size();
  string name = "v" + to_string(var);
  variable_to_int[name] = var;
  int_to_variable[var] = name;
  return var;
}

bool MatchesRNEq1(const Formula &f) {
  return f.IsOp(op_not) && f.Subformula().IsOp(op_equiv) &&
         f.Subformula().Subformula(0).IsVar();
}
vector<Set> RNEq1(const Formula &f) {
  if (MatchesRNEq1(f)) {
    Formula new_var(GetNewVar());
    return {
        Set({Formula(op_not,
                     {{op_equiv, {{f.Subformula().Subformula(0), new_var}}}}),
             Formula(op_equiv, {{new_var, f.Subformula().Subformula(1)}})})};
  }
  return {};
}

bool MatchesRNEq2(const Formula &f) {
  return f.IsOp(op_not) && f.Subformula().IsOp(op_equiv);
}
vector<Set> RNEq2(const Formula &f) {
  if (MatchesRNEq2(f)) {
    Formula alpha(GetNewVar());
    Formula beta(GetNewVar());
    return {Set({Formula(op_equiv, {{alpha, f.Subformula().Subformula(0)}}),
                 Formula(op_equiv, {{beta, f.Subformula().Subformula(1)}}),
                 Formula(op_not, {{Formula(op_equiv, {{alpha, beta}})}})})};
  }
  return {};
}

bool MatchesREqNot(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_not);
}
vector<Set> REqNot(const Formula &f) {
  if (MatchesREqNot(f)) {
    Formula alpha(GetNewVar());
    return {
        Set({Formula(op_equiv, {{f.Subformula(0), Formula(op_not, {{alpha}})}}),
             Formula(op_equiv, {{alpha, f.Subformula(1).Subformula()}})})};
  }
  return {};
}

bool MatchesREqImpl(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_impl);
}
bool MatchesREqImplLeft(const Formula &f) {
  return MatchesREqImpl(f) && f.Subformula(1).Subformula(0).IsVar();
}
bool MatchesREqImplRight(const Formula &f) {
  return MatchesREqImpl(f) && f.Subformula(1).Subformula(1).IsVar();
}
vector<Set> REqImpl(const Formula &f) {
  if (MatchesREqImpl(f)) {
    Formula alpha(GetNewVar());
    Formula beta(GetNewVar());
    return {Set({Formula(op_equiv, {{f.Subformula(0),
                                     Formula(op_impl, {{alpha, beta}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(0)}}),
                 Formula(op_equiv, {{beta, f.Subformula(1).Subformula(1)}})})};
  }
  return {};
}
vector<Set> REqImplLeft(const Formula &f) {
  if (MatchesREqImplLeft(f)) {
    Formula alpha(GetNewVar());
    return {Set({Formula(op_equiv,
                         {{f.Subformula(0),
                           Formula(op_impl,
                                   {{f.Subformula(1).Subformula(0), alpha}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(1)}})})};
  }
  return {};
}
vector<Set> REqImplRight(const Formula &f) {
  if (MatchesREqImplRight(f)) {
    Formula alpha(GetNewVar());
    return {Set({Formula(op_equiv,
                         {{f.Subformula(0),
                           Formula(op_impl,
                                   {{alpha, f.Subformula(1).Subformula(1)}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(0)}})})};
  }
  return {};
}