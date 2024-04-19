#pragma once

#include "commons.h"
#include "label_rules.h"
#include "label_set.h"
#include "solver.h"

vector<LabelNode> ApplyRule(LabelNode &n, LabelRule rule, bool is_positive);

bool ApplyEqRule(LabelNode &n, LabelEqRule rule);

void LabelSolve(LabelNode &n);

LabelNode DoSolveLabel(string input_string, bool print);
LabelNode DoSolveLabel(vector<Formula> formulas, bool print);

void TestInput(string input_string, bool print = true);
bool TestFormula(const Formula &f, bool print = true);