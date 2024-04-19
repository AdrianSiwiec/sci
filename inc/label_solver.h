#pragma once

#include "commons.h"
#include "label_rules.h"
#include "label_set.h"

vector<LabelNode> ApplyRule(LabelNode &n, LabelRule rule,
                            bool is_positive);

void LabelSolve(LabelNode &n);