#pragma once

#include "commons.h"

enum Operator { op_not, op_impl, op_equiv };

struct Formula {
  Formula(int var);
  Formula(Operator op, const vector<Formula> subformulas);

  bool IsVar() const;
  bool IsVar(int var) const;
  int Var() const;
  bool IsOp(Operator op) const;
  Operator Op() const;
  vector<Formula> Subformulas() const;
  Formula Subformula(int i = 0) const;

private:
  // A formula is a variable, XOR
  bool is_var;
  int var;
  // An operator with one or two subformulas.
  Operator op;
  vector<Formula> subformulas;
};

ostream &operator<<(ostream &os, const Formula &f);
bool operator==(const Formula &a, const Formula &b);
bool operator!=(const Formula &a, const Formula &b);
bool operator<(const Formula &a, const Formula &b);
bool operator>(const Formula &a, const Formula &b);

struct Set {
  Set(vector<Formula> formulas);
  vector<Formula> formulas;
};

struct ProofNode {
  Set root;
  vector<ProofNode> subnodes;
};