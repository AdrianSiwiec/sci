#include "structs.h"

#define CHECK_POS                                                              \
  if (pos >= s.size())                                                         \
    return nullopt;

optional<int> ParseVar(const string &s, int &pos) {
  CHECK_POS;
  if (!(s[pos] >= '0' && s[pos] <= '9'))
    return nullopt;
  string to_parse = s.substr(pos);
  size_t parsed_chars;
  int value;
  try {
    value = stoi(to_parse, &parsed_chars);
  } catch (...) {
    return nullopt;
  }
  pos += parsed_chars;
  return value;
}
optional<Operator> ParseOp(const string &s, int &pos) {
  CHECK_POS;
  if (s[pos] == '=') {
    pos++;
    return op_equiv;
  }
  if (s[pos] == '-' && (pos + 1 < s.size()) && s[pos + 1] == '>') {
    pos += 2;
    return op_impl;
  }
  return nullopt;
}

optional<Formula> Formula::ParseOpFormula(const string &s, int &pos) {
  Formula f(0);
  f.is_var = false;
  int original_pos = pos;

  // First Subformula
  optional<Formula> child = Parse(s, pos);
  if (!child.has_value()) {
    pos = original_pos;
    return nullopt;
  }
  f.subformulas.push_back(child.value());

  // Operator
  optional<Operator> op = ParseOp(s, pos);
  if (!op.has_value()) {
    pos = original_pos;
    return nullopt;
  }
  f.op = op.value();

  // Second Subformula
  child = Parse(s, pos);
  if (!child.has_value()) {
    pos = original_pos;
    return nullopt;
  }
  f.subformulas.push_back(child.value());
  return f;
}

optional<Formula> Formula::Parse(const string &s, int &pos) {
  Formula f(0);
  CHECK_POS;

  bool got_bracket = false;
  if (s[pos] == '(') {
    pos++;
    got_bracket = true;
  }

  CHECK_POS;
  if (got_bracket) {
    auto f2op = ParseOpFormula(s, pos);
    if (f2op.has_value()) {
      if (s.size() >= pos && s[pos] == ')') {
        pos++;
        return f2op.value();
      }
    }
  }

  int save_pos = pos;
  optional<int> var;
  if ((var = ParseVar(s, pos))) { // TODO: all vars are (num)
    if (!(got_bracket && s.size() > pos && s[pos] != ')')) {
      f.is_var = true;
      f.var = var.value();
      if (got_bracket)
        pos++;
      return f;
    }
    pos = save_pos;
  }

  save_pos = pos;
  if (s[pos] == '-') {
    pos++;
    optional<Formula> child = Parse(s, pos);
    if (child.has_value()) {
      if (!(got_bracket && s.size() > pos && s[pos] != ')')) {
        f.is_var = false;
        f.op = op_not;
        f.subformulas.push_back(child.value());
        if (got_bracket)
          pos++;
        return f;
      }
    }
    pos = save_pos;
  }

  // To handle more brackets than needed
  if (got_bracket) {
    auto f = Parse(s, pos);
    if (f.has_value()) {
      pos++;
      return f;
    }
    pos--;
  }
  return nullopt;
}