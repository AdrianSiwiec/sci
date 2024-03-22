#include "structs.h"

#define CHECK_POS                                                              \
  if (pos >= s.size())                                                         \
    return nullopt;

optional<int> ParseVar(const string &s, int &pos) {
  CHECK_POS;
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

optional<Formula> Formula::Parse(const string &s, int &pos) {
  Formula f(0);
  CHECK_POS;

  if (s[pos] == '(') {
    pos++;
  } else {
    return nullopt;
  }

  CHECK_POS;
  if (s[pos] >= '0' && s[pos] <= '9') { // TODO: all vars are (num)
    f.is_var = true;
    auto var = ParseVar(s, pos);
    if (!var.has_value())
      return nullopt;
    f.var = var.value();
  } else if (s[pos] == '-') {
    f.is_var = false;
    f.op = op_not;
    pos++;
    optional<Formula> child = Parse(s, pos);

    if (!child.has_value())
      return nullopt;
    assert(f.subformulas.empty());
    f.subformulas.push_back(child.value());
  } else {
    f.is_var = false;

    // First Subformula
    CHECK_POS;
    optional<Formula> child = Parse(s, pos);
    if (!child.has_value())
      return nullopt;
    f.subformulas.push_back(child.value());

    // Operator
    CHECK_POS;
    optional<Operator> op = ParseOp(s, pos);
    if (!op.has_value())
      return nullopt;
    f.op = op.value();

    // Second Subformula
    CHECK_POS;
    assert(f.subformulas.size() == 1);

    child = Parse(s, pos);
    if (!child.has_value())
      return nullopt;
    f.subformulas.push_back(child.value());
  }

  if (s[pos] == ')') {
    pos++;
  } else {
    return nullopt;
  }
  return f;
}