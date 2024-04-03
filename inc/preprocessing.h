#pragma once

#include "commons.h"
#include "structs.h"

extern map<string, int> variable_to_int;
extern map<int, string> int_to_variable;

void ClearVars();

void ProcessVarChar(string &s, int &pos);
string PreprocessInput(string s);

optional<Formula> ParseInput(string s);

Formula PostprocessFormula(const Formula &f);