#pragma once

#include "commons.h"
#include "label_rules.h"
#include "label_set.h"

vector<LabelNode> ApplyRule(LabelNode &n, LabelRule rule,
                            bool is_positive);

bool ApplyEqRule(LabelNode &n, LabelEqRule rule);

void LabelSolve(LabelNode &n);