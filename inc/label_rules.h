#pragma once

#include "commons.h"
#include "label_set.h"

typedef vector<LabelNode>(LabelRule)(const LabelNode &n, int label);

// LabelRules are only the decompositon rules. Other rules are hardcoded in the
// LabelSolve method.

LabelRule LRNotPlus;
LabelRule LRNotMinus;
LabelRule LRImplPlus;
LabelRule LRImplMinus;
LabelRule LRIdPlus;
LabelRule LRIdMinus;

string GetLabelRuleString(LabelRule rule);
