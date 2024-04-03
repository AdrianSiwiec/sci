#pragma once

#include "commons.h"

enum Operator : char {
  // Canonical operators
  op_not,
  op_impl,
  op_id,
  // Additional operators that will be replaced by the canonical ones after
  // parsing
  op_and,
  op_or,
  op_equiv
};

struct Formula {
  Formula(int var);
  Formula(Operator op, const vector<Formula> subformulas);
  Formula(string input);

  bool IsVar() const;
  bool IsVar(int var) const;
  int Var() const;
  bool IsOp(Operator op) const;
  Operator Op() const;
  vector<Formula> Subformulas() const;
  Formula Subformula(int i = 0) const;

  // Implemented in parser.cpp
  static optional<Formula> Parse(const string &s, int &pos);

private:
  // A formula is a variable, XOR
  bool is_var;
  int var;
  // An operator with one or two subformulas.
  Operator op;
  vector<Formula> subformulas;
  static optional<Formula> ParseOpFormula(const string &s, int &pos);
  void Normalize();
};

ostream &operator<<(ostream &os, const Formula &f);
bool operator==(const Formula &a, const Formula &b);
bool operator!=(const Formula &a, const Formula &b);
bool operator<(const Formula &a, const Formula &b);
bool operator>(const Formula &a, const Formula &b);

Formula FNot(const Formula &f);
Formula FImpl(const Formula &a, const Formula &b);
Formula FId(const Formula &a, const Formula &b);

optional<int> ParseVar(const string &s, int &pos);
optional<Operator> ParseOp(const string &s, int &pos);

struct Set {
  Set(vector<Formula> formulas);
  Set(string input);
  void ReplaceFormula(int index, Formula f);
  void RemoveFormula(int index);
  void AddFormula(Formula f);
  vector<Formula> Formulas() const;

private:
  void Normalize();
  vector<Formula> formulas;
};

ostream &operator<<(ostream &os, const Set &s);
bool operator==(const Set &a, const Set &b);
bool operator!=(const Set &a, const Set &b);
bool operator<(const Set &a, const Set &b);

vector<Set> ParseSets(string input);

struct ProofNode {
  ProofNode(Set root) : root(root) {}

  Set root;
  vector<ProofNode> subnodes;
  optional<bool> is_closed = nullopt;

  optional<string> rule_used;
  optional<Formula> formula_used;
};

bool operator<(const ProofNode &a, const ProofNode &b);
bool operator==(const ProofNode &a, const ProofNode &b);

void PrintProofNode(const ProofNode &n, string prefix = "");