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

bool operator==(const Formula &a, const Formula &b) {
  if (a.IsVar() != b.IsVar())
    return false;
  if (a.IsVar())
    return a.Var() == b.Var();
  if (a.Op() != b.Op())
    return false;
  assert(a.Subformulas().size() == b.Subformulas().size());
  for (int i = 0; i < a.Subformulas().size(); i++) {
    if (!(a.Subformula(i) == b.Subformula(i)))
      return false;
  }
  return true;
}

bool operator!=(const Formula &a, const Formula &b) { return !(a == b); }

bool operator<(const Formula &a, const Formula &b) {
  if (a.IsVar() != b.IsVar())
    return a.IsVar() > b.IsVar();
  if (a.IsVar())
    return a.Var() < b.Var();
  if (a.Op() != b.Op())
    return a.Op() < b.Op();
  assert(a.Subformulas().size() == b.Subformulas().size());
  for (int i = 0; i < a.Subformulas().size(); i++) {
    if (!(a.Subformula(i) == b.Subformula(i)))
      return a.Subformula(i) < b.Subformula(i);
  }
  return 0 < 0;
}
bool operator>(const Formula &a, const Formula &b) { return b < a; }

Set::Set(vector<Formula> formulas) : formulas(formulas) {
  sort(this->formulas.begin(), this->formulas.end());
}

vector<Formula> Set::Formulas() const {
  return formulas;
}

void Set::ReplaceFormula(int index, Formula f) {
  assert(formulas.size() > index);
  formulas[index] = f;
  sort(formulas.begin(), formulas.end());
}

bool operator==(const Set &a, const Set &b) { return a.Formulas() == b.Formulas(); }