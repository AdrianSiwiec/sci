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

// - -f
// ----
// {f}
Rule RNot;

// f -> g
// ----------
// {-f} | {g}
Rule RImpl;

const vector<Rule *> AllRules{RNot, RImpl};
