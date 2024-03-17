#include "structs.h"

Formula::Formula(int var) : is_var(true), var(var) {}
Formula::Formula(Operator op, const vector<Formula> subformulas)
    : is_var(false), op(op), subformulas(subformulas) {}

bool Formula::IsVar() const { return is_var; }

bool Formula::IsVar(int var) const { return is_var && this->var == var; }

int Formula::Var() const {
  assert(IsVar());
  return var;
}

bool Formula::IsOp(Operator op) const { return !is_var && this->op == op; }

Operator Formula::Op() const {
  assert(!is_var);
  return op;
}

vector<Formula> Formula::Subformulas() const {
  assert(!is_var);
  return subformulas;
}

Formula Formula::Subformula(int i) const {
  assert(!is_var);
  assert(subformulas.size() > i);
  return subformulas[i];
}

ostream &operator<<(ostream &os, const Formula &f) {
  if (f.IsVar()) {
    os << "v" << f.Var();
  } else {
    assert(!f.Subformulas().empty());
    os << "(";
    if (f.IsOp(op_not))
      os << "-";
    os << f.Subformula(0);
    switch (f.Op()) {
    case op_not:
      break;
    case op_impl:
      os << "->";
      break;
    case op_equiv:
      os << "=";
      break;
    default:
      assert(false);
    }
    if (f.Subformulas().size() > 1)
      os << f.Subformula(1);
    os << ")";
    assert(f.Subformulas().size() < 3);
  }
  return os;
}

Set::Set(vector<Formula> formulas) : formulas(formulas) {}