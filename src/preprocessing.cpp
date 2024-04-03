#include "preprocessing.h"

map<string, int> variable_to_int;
map<int, string> int_to_variable;

void ClearVars() {
  variable_to_int.clear();
  int_to_variable.clear();
}

bool IsVariableChar(char c) {
  // return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
  //  (c >= '0' && c <= '9');
  if (c == '-' || c == '<' || c == '>' || c == '=' || c == '(' || c == ')' ||
      c == '!' || c == '|' || c == '&' || c == '^' || c == '*')
    return false;
  return true;
}

void ProcessVarChar(string &s, int &pos) {
  int len = 0;
  while (len < s.size() && IsVariableChar(s[pos + len])) {
    len++;
  }
  string var = s.substr(pos, len);
  int new_var;
  if (variable_to_int.count(var) == 0) {
    new_var = variable_to_int.size();
    variable_to_int[var] = new_var;
    int_to_variable[new_var] = var;
  } else {
    new_var = variable_to_int[var];
  }

  s.replace(pos, len, to_string(new_var));
  pos += to_string(new_var).size();
}

string PreprocessInput(string s) {
  // Remove whitespaces
  s.erase(remove_if(s.begin(), s.end(), [](char x) { return isspace(x); }),
          s.end());

  for (int pos = 0; pos < s.size(); pos++) {
    if (IsVariableChar(s[pos])) {
      ProcessVarChar(s, pos);
    }
  }

  return "(" + s + ")";
}

optional<Formula> ParseInputFormula(string s) {
  string s1 = PreprocessInput(s);
  int pos = 0;
  return Formula::Parse(s1, pos);
}

Formula PostprocessFormula(const Formula &f) {
  if (f.IsVar())
    return f;
  if (f.IsOp(op_not)) {
    return Formula(op_not, {PostprocessFormula(f.Subformula())});
  }
  Formula a = PostprocessFormula(f.Subformula(0));
  Formula b = PostprocessFormula(f.Subformula(1));
  if (f.IsOp(op_impl) || f.IsOp(op_id)) {
    return Formula(f.Op(), {a, b});
  }
  if (f.IsOp(op_and)) {
    return Formula(op_not, {Formula(op_impl, {a, Formula(op_not, {{b}})})});
  }
  if (f.IsOp(op_or)) {
    return Formula(op_impl, {Formula(op_not, {a}), b});
  }
  if (f.IsOp(op_equiv)) {
    return Formula(
        op_not,
        {Formula(op_impl, {Formula(op_impl, {a, b}),
                           Formula(op_not, {Formula(op_impl, {b, a})})})});
  }
  return Formula(0);
}