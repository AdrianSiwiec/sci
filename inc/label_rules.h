#pragma once

#include "commons.h"
#include "label_set.h"

typedef vector<LabelNode>(LabelRule)(const LabelNode &n, int label);

// LabelRules are only the decompositon rules.
LabelRule LRNotPlus;