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

// ----------------------- Classical rules -----------------------
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

// ----------------------- Equality Rules -----------------------
bool IsSimple(const Formula &f);

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

// p = (q = f)
// ------------------
// p = (q = a), a = f
Rule REqEqLeft;

// p = (f = q)
// ------------------
// p = (a = q), a = f
Rule REqEqRight;

// p = (f = g)
// -------------------------
// p = (a = b), a = f, b = g
Rule REqEq;

// f = g
// -------------------
// a = b, a = f, b = g
Rule REq;

// ----------------------- Termination Rules -----------------------

// p = q
// ---------------------------
// p, q, p = q | -p, -q, p = q
Rule RTerEq;

// p = -q
// -----------------------------
// p, -q, p = -q | -p, q, p = -q
Rule RTerEqNot;

// p = (q -> r)
// ---------------------------------
// p, -q, f | p, r, f | -p, q, -r, f   // f is p = (q -> r)
Rule RTerEqImpl;

// p = (q = r)
// ---------------------------
// F1 | F2 | F3 | F4 | F5 | F6
Rule RTerEqEq;

// -(p = q)
// -----------------
// F1 | F2 | F3 | F4
Rule RTerSpike;

Formula ReplaceAll(const Formula &f, const Formula &to_replace,
                   const Formula &replace_with);

int GetNewVar();

bool IsSingleUseRule(Rule r);
bool WasRuleApplied(Rule r, const Formula &f);
void MarkRuleAsApplied(Rule r, const Formula &f);
void ClearAppliedRules();

string GetRuleName(Rule r);

const vector<Rule *> AllRules{RNot, RNotImpl, RImpl, nullptr,
                              // Equality Rules
                              RNEq1, RNEq2, REqNot, REqImplLeft, REqImplRight,
                              REqImpl, REqEqLeft, REqEqRight, REqEq, REq,
                              // Termination Rules
                              RTerEq, RTerEqNot, RTerEqImpl, RTerEqEq,
                              RTerSpike};
