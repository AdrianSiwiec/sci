#pragma once

#include "commons.h"
#include "rules.h"
#include "structs.h"

// If worked, returns non-empty.
vector<ProofNode>
ApplyRule(ProofNode& node, Rule rule,
          const ProofNode &previous = ProofNode(Set(vector<Formula>())));

vector<ProofNode> BuildChildNodes(const ProofNode &node, int formula_to_remove,
                                  vector<Set> formulas_to_add);

bool IsAx1(const Formula &f);
bool IsAx2(const Formula &f, const Formula &g);
bool IsClosed(const Set &set);
bool IsClosed(ProofNode &n);

// Will build a tree.
void Solve(ProofNode &n,
           ProofNode previous = ProofNode(Set(vector<Formula>())));

// Solve with printing etc.
void DoSolve(string s);