#pragma once

#include "commons.h"
#include "structs.h"

int GetRandom(int start, int end);
int GetRandom(int end);

Formula GetRandomFormula(int subformulas, int variables_size,
                         const vector<Formula> &interesting = {});
Formula GetRandomKRZFormula(int subformulas, int variables_size,
                            const vector<Formula> &vars);