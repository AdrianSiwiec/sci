#include "structs.h"
#include "preprocessing.h"
#include "solver.h"

Formula::Formula(int var) : is_var(true), var(var) {}
Formula::Formula(Operator op, const vector<Formula> subformulas)
    : is_var(false), op(op), subformulas(subformulas) {
  Normalize();
}
Formula::Formula(string input) {
  optional<Formula> f = ParseInput(input);
  if (!f.has_value()) {
    cerr << "Could not parse \"" << input << "\" as formula!" << endl;
    assert(false);
  }
  *this = f.value();
  Normalize();
}

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

void Formula::Normalize() {
  if (is_var == false) {
    if (op == op_not)
      assert(this->subformulas.size() == 1);
    else
      assert(this->subformulas.size() == 2);
  }
  if (IsOp(op_equiv)) {
    sort(subformulas.begin(), subformulas.end());
  }
}

ostream &operator<<(ostream &os, const Formula &f) {
  if (f.IsVar()) {
    if (int_to_variable.count(f.Var()) > 0) {
      os << int_to_variable[f.Var()];
    } else {
      os << "v" << f.Var();
    }
  } else {
    assert(!f.Subformulas().empty());
    if (!f.IsOp(op_not))
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
    if (!f.IsOp(op_not))
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

Set::Set(vector<Formula> formulas) : formulas(formulas) { this->Normalize(); }
Set::Set(string input) {
  vector<string> input_formulas = SplitString(input, ",");
  for (auto s : input_formulas) {
    formulas.push_back(Formula(s));
  }
  this->Normalize();
}

vector<Formula> Set::Formulas() const { return formulas; }

void Set::Normalize() {
  sort(formulas.begin(), formulas.end());
  auto last = unique(formulas.begin(), formulas.end());
  formulas.erase(last, formulas.end());
}

void Set::ReplaceFormula(int index, Formula f) {
  assert(formulas.size() > index);
  formulas[index] = f;
  Normalize();
}

void Set::RemoveFormula(int index) {
  assert(formulas.size() > index);
  swap(formulas[index], formulas[formulas.size() - 1]);
  formulas.pop_back();
  Normalize();
}

void Set::AddFormula(Formula f) {
  formulas.push_back(f);
  Normalize();
}

vector<Set> ParseSets(string input) {
  vector<Set> result;
  auto strings = SplitString(input, "|");
  for (auto s : strings) {
    result.push_back(Set(s));
  }
  return result;
}

bool operator==(const Set &a, const Set &b) {
  return a.Formulas() == b.Formulas();
}
bool operator!=(const Set &a, const Set &b) { return !(a == b); }

ostream &operator<<(ostream &os, const Set &s) {
  os << "{";
  for (int i = 0; i < s.Formulas().size(); i++) {
    os << s.Formulas()[i];
    if (i + 1 < s.Formulas().size())
      os << ", ";
  }
  os << "}";
  return os;
}

void PrintProofNode(const ProofNode &n, string prefix) {
  cout << prefix << "--: " << n.root << "\t(is "
       << (n.is_closed ? "closed" : "open") << ")" << endl;
  for (int i = 0; i < n.subnodes.size(); i++) {
    string add_to_prefix = ((i + 1 < n.subnodes.size()) ? " |" : " |");
    PrintProofNode(n.subnodes[i], prefix + add_to_prefix);
  }
}