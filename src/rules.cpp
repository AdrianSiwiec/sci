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

bool IsSimple(const Formula &f) {
  if (f.IsVar())
    return true;
  if (f.IsOp(op_not) && f.Subformula().IsVar())
    return true;
  if (f.IsOp(op_equiv) && f.Subformula(0).IsVar() && f.Subformula(1).IsVar())
    return true;
  if (f.IsOp(op_not) && f.Subformula().IsOp(op_equiv) &&
      f.Subformula().Subformula(0).IsVar() &&
      f.Subformula().Subformula(1).IsVar())
    return true;
  if (f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
      f.Subformula(1).IsOp(op_not) && f.Subformula(1).Subformula().IsVar())
    return true;
  if (f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
      (f.Subformula(1).IsOp(op_equiv) || f.Subformula(1).IsOp(op_impl)) &&
      f.Subformula(1).Subformula(0).IsVar() &&
      f.Subformula(1).Subformula(1).IsVar())
    return true;
  return false;
}

bool MatchesRNEq1(const Formula &f) {
  if (IsSimple(f))
    return false;
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
  if (IsSimple(f))
    return false;
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
  if (IsSimple(f))
    return false;
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
  if (IsSimple(f))
    return false;
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

bool MatchesREqEq(const Formula &f) {
  if (IsSimple(f))
    return false;
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_equiv);
}
bool MatchesREqEqLeft(const Formula &f) {
  return MatchesREqEq(f) && f.Subformula(1).Subformula(0).IsVar();
}
bool MatchesREqEqRight(const Formula &f) {
  return MatchesREqEq(f) && f.Subformula(1).Subformula(1).IsVar();
}
vector<Set> REqEq(const Formula &f) {
  if (MatchesREqEq(f)) {
    Formula alpha(GetNewVar());
    Formula beta(GetNewVar());
    return {Set({Formula(op_equiv, {{f.Subformula(0),
                                     Formula(op_equiv, {{alpha, beta}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(0)}}),
                 Formula(op_equiv, {{beta, f.Subformula(1).Subformula(1)}})})};
  }
  return {};
}
vector<Set> REqEqLeft(const Formula &f) {
  if (MatchesREqEqLeft(f)) {
    Formula alpha(GetNewVar());
    return {Set({Formula(op_equiv,
                         {{f.Subformula(0),
                           Formula(op_equiv,
                                   {{f.Subformula(1).Subformula(0), alpha}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(1)}})})};
  }
  return {};
}
vector<Set> REqEqRight(const Formula &f) {
  if (MatchesREqEqRight(f)) {
    Formula alpha(GetNewVar());
    return {Set({Formula(op_equiv,
                         {{f.Subformula(0),
                           Formula(op_equiv,
                                   {{alpha, f.Subformula(1).Subformula(1)}})}}),
                 Formula(op_equiv, {{alpha, f.Subformula(1).Subformula(0)}})})};
  }
  return {};
}

bool MatchesREq(const Formula &f) {
  if (IsSimple(f))
    return false;
  return f.IsOp(op_equiv);
}
vector<Set> REq(const Formula &f) {
  if (MatchesREq(f)) {
    Formula alpha(GetNewVar());
    Formula beta(GetNewVar());
    return {Set({Formula(op_equiv, {{alpha, beta}}),
                 Formula(op_equiv, {{alpha, f.Subformula(0)}}),
                 Formula(op_equiv, {{beta, f.Subformula(1)}})})};
  }
  return {};
}

bool MatchesRTerEq(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() && f.Subformula(1).IsVar();
}
vector<Set> RTerEq(const Formula &f) {
  if (MatchesRTerEq(f)) {
    return {Set({f.Subformula(0), f.Subformula(1), f}),
            Set({Formula(op_not, {{f.Subformula(0)}}),
                 Formula(op_not, {{f.Subformula(1)}}), f})};
  }
  return {};
}
bool MatchesRTerEqNot(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_not) && f.Subformula(1).Subformula().IsVar();
}
vector<Set> RTerEqNot(const Formula &f) {
  if (MatchesRTerEqNot(f)) {
    return {Set({f.Subformula(0), f.Subformula(1), f}),
            Set({Formula(op_not, {{f.Subformula(0)}}),
                 f.Subformula(1).Subformula(), f})};
  }
  return {};
}
bool MatchesRTerEqImpl(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_impl) &&
         f.Subformula(1).Subformula(0).IsVar() &&
         f.Subformula(1).Subformula(1).IsVar();
}
vector<Set> RTerEqImpl(const Formula &f) {
  if (MatchesRTerEqImpl(f)) {
    Formula p = f.Subformula(0);
    Formula q = f.Subformula(1).Subformula(0);
    Formula r = f.Subformula(1).Subformula(1);
    return {Set({p, Formula(op_not, {{q}}), f}), Set({p, r, f}),
            Set({Formula(op_not, {{p}}), q, Formula(op_not, {{r}}), f})};
  }
  return {};
}
bool MatchesRTerEqEq(const Formula &f) {
  return f.IsOp(op_equiv) && f.Subformula(0).IsVar() &&
         f.Subformula(1).IsOp(op_equiv) &&
         f.Subformula(1).Subformula(0).IsVar() &&
         f.Subformula(1).Subformula(1).IsVar();
}
vector<Set> RTerEqEq(const Formula &f) {
  if (MatchesRTerEqEq(f)) {
    Formula p = f.Subformula(0);
    Formula q = f.Subformula(1).Subformula(0);
    Formula r = f.Subformula(1).Subformula(1);
    return {
        // F1
        Set({p, q, r, Formula(op_equiv, {{q, r}}), f}),
        // F2
        Set({p, Formula(op_not, {{q}}), Formula(op_not, {{r}}),
             Formula(op_equiv, {{q, r}}), f}),
        // F3
        Set({Formula(op_not, {{p}}), q, r,
             Formula(op_not, {{Formula(op_equiv, {{q, r}})}}), f}),
        // F4
        Set({Formula(op_not, {{p}}), q, Formula(op_not, {{r}}),
             Formula(op_not, {{Formula(op_equiv, {{q, r}})}}), f}),
        // F5
        Set({Formula(op_not, {{p}}), Formula(op_not, {{q}}), r,
             Formula(op_not, {{Formula(op_equiv, {{q, r}})}}), f}),
        // F6
        Set({Formula(op_not, {{p}}), Formula(op_not, {{q}}),
             Formula(op_not, {{r}}),
             Formula(op_not, {{Formula(op_equiv, {{q, r}})}}), f}),
    };
  }
  return {};
}
bool MatchesRTerSpike(const Formula &f) {
  return f.IsOp(op_not) && f.Subformula().IsOp(op_equiv) &&
         f.Subformula().Subformula(0).IsVar() &&
         f.Subformula().Subformula(1).IsVar();
}
vector<Set> RTerSpike(const Formula &f) {
  if (MatchesRTerSpike(f)) {
    Formula p = f.Subformula().Subformula(0);
    Formula q = f.Subformula().Subformula(1);
    return {Set({p, q, f}), Set({p, Formula(op_not, {{q}}), f}),
            Set({Formula(op_not, {{p}}), q, f}),
            Set({Formula(op_not, {{p}}), Formula(op_not, {{q}}), f})};
  }
  return {};
}
