#pragma once

#include "commons.h"
#include "rules.h"
#include "structs.h"

// If worked, returns non-empty.
vector<ProofNode> ApplyRule(const ProofNode node, Rule rule);

vector<ProofNode> BuildChildNodes(const ProofNode &node, int formula_to_remove,
                                  vector<Set> formulas_to_add);

bool IsAx1(const Formula &f);
bool IsAx2(const Formula &f, const Formula &g);
bool IsClosed(const Set &set);

// Will build a tree.
bool IsClosed(ProofNode &n);