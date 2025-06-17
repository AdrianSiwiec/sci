#pragma once

#include "commons.h"
#include "label_rules.h"
#include "label_set.h"
#include "solver.h"
#include "stats.h"

vector<LabelNode> ApplyRule(LabelNode &n, LabelRule rule, bool is_positive);

bool ApplyEqRule(LabelNode &n, LabelEqRule rule);

void LabelSolve(LabelNode &n);

LabelNode DoSolveLabel(string input_string,
                       StatsAtom &statsAtom = dummyStatsAtom,
                       bool print = false);
LabelNode DoSolveLabel(const vector<Formula> &formulas,
                       StatsAtom &statsAtom = dummyStatsAtom,
                       bool print = false);

void TestInput(string input_string, bool print = true);
bool TestFormula(const Formula &f, bool print = true);