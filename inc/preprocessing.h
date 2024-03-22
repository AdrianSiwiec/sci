#pragma once

#include "commons.h"

extern map<string, int> variable_to_int;
extern map<int, string> int_to_variable;

void ClearVars();

void ProcessVarChar(string &s, int &pos);
string PreprocessInput(string s);