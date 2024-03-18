#pragma once

#include "commons.h"
#include "rules.h"
#include "structs.h"

// If worked, returns non-empty.
vector<ProofNode> ApplyRule(const ProofNode &node, Rule rule);

void PopulateSubnodes(ProofNode &node);
vector<ProofNode> BuildChildNodes(const ProofNode &node, int formula_to_remove,
                                   vector<Set> formulas_to_add);