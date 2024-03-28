#pragma once

#include "commons.h"
#include "structs.h"

typedef vector<Set>(Rule)(const Formula &f);

// All rules (but RuleFun) here are simplified: they take a formula and if it
// matches, they return a vector of sets of resulting formulas. The meaning is:
// f
// ------------
// {f, g} | {h}
// means that the rule says:
// F u {f}
// --------------------
// F u {f, g} | F u {h}
// where u is short for \cup
// f,g are formulas.
// p,q are variables.
// a,b are new variables, introduced by the conclusion

// - -f
// ----
// {f}
Rule RNot;

// f -> g
// ----------
// {-f} | {g}
Rule RImpl;

// -(f -> g)
// ---------
// {f, -g}
Rule RNotImpl;

// p = f
// ----------------
// F(f/p) u {p = f}
vector<Set> RFun(const Formula &f, Set s);

// TODO: "Wyróżniona formuła nie jest prosta"???

// p != f
// ---------------
// {p != a, a = f}
Rule RNEq1;

// f != g
// ----------------
// {a=f, b=g, a!=b}
Rule RNEq2;

// p = -f
// -------------
// p = -a, a = f
Rule REqNot;

// p = (q -> f)
// -------------------
// p = (q -> a), a = f
Rule REqImplLeft;

// p = (f -> q)
// -------------------
// p = (a -> q), a = f
Rule REqImplRight;

// p = (f -> g)
// --------------------
// p = (a->b), a=f, b=g
Rule REqImpl;

Formula ReplaceAll(const Formula &f, const Formula &to_replace,
                   const Formula &replace_with);

int GetNewVar();

// TODO: the order is not exactly as the order they were listed in.
const vector<Rule *> AllRules{RNot,  RImpl, RNotImpl, nullptr,
                              RNEq1, RNEq2, REqNot};
