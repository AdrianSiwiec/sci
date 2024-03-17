#include "structs.h"

Formula::Formula(int var) : is_var(true), var(var) {}
Formula::Formula(Operator op, const vector<Formula> &subformulas)
    : is_var(false), op(op), subformulas(subformulas) {}

ostream &operator<<(ostream &os, const Formula &f) {
  if (f.is_var) {
    os << "v" << f.var;
  } else {
    assert(!f.subformulas.empty());
    os << "(";
    if (f.op == op_not)
      os << "-";
    os << f.subformulas[0];
    switch (f.op) {
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
    if (f.subformulas.size() > 1)
      os << f.subformulas[1];
    os << ")";
    assert(f.subformulas.size() < 3);
  }
  return os;
}