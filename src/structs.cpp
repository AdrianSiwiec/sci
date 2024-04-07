#include "structs.h"
#include "preprocessing.h"
#include "solver.h"

Formula::Formula(int var) : is_var(true), var(var) { Normalize(); }
Formula::Formula(Operator op, const vector<Formula> &subformulas)
    : is_var(false), op(op), subformulas(subformulas) {
  Normalize();
}
Formula::Formula(string input) {
  optional<Formula> f = ParseInputFormula(input);
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

const vector<Formula> &Formula::Subformulas() const {
  assert(!is_var);
  return subformulas;
}

const Formula &Formula::Subformula(int i) const {
  assert(!is_var);
  assert(subformulas.size() > i);
  return subformulas[i];
}

ull Formula::Hash() const { return hash; }

void Formula::Normalize() {
  if (is_var == false) {
    if (op == op_not)
      assert(this->subformulas.size() == 1);
    else
      assert(this->subformulas.size() == 2);
  }
  if (IsOp(op_id) && subformulas[0] > subformulas[1]) {
    swap(subformulas[0], subformulas[1]);
    // sort(subformulas.begin(), subformulas.end());
  }

  // if (is_var) {
  //   hash = GetHash(var + 10, {});
  // } else {
  //   vector<ull> subhashes;
  //   for (const auto &sf : subformulas) {
  //     subhashes.push_back(sf.hash);
  //   }
  //   hash = GetHash(op, subhashes);
  // }

  //   ull GetHash(ull root, const vector<ull> &subhashes) {
  //   ull result = (root + 17) * 43;
  //   for (const ull subhash : subhashes) {
  //     result += subhash;
  //     result *= 13;
  //   }
  //   return result;
  // }

  if (is_var) {
    hash = (var + 10) * 43;
  } else {
    hash = (op + 17000000) * 47;
    for (const auto &sf : subformulas) {
      hash += sf.hash;
      hash *= 239;
    }
  }

  // cout << "Hash of " << *this << " : " << hash << endl;
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
      os << "¬";
    os << f.Subformula(0);
    switch (f.Op()) {
    case op_not:
      break;
    case op_impl:
      os << "→";
      break;
    case op_id:
      os << "≡";
      break;
    case op_and:
      os << "∧";
      break;
    case op_or:
      os << "∨";
      break;
    case op_equiv:
      os << "↔";
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
  return a.hash == b.hash;
  // if (a.IsVar() != b.IsVar())
  //   return false;
  // if (a.IsVar())
  //   return a.Var() == b.Var();
  // if (a.Op() != b.Op())
  //   return false;
  // assert(a.Subformulas().size() == b.Subformulas().size());
  // for (int i = 0; i < a.Subformulas().size(); i++) {
  //   if (!(a.Subformula(i) == b.Subformula(i)))
  //     return false;
  // }
  // return true;
}

bool operator!=(const Formula &a, const Formula &b) { return !(a == b); }

bool operator<(const Formula &a, const Formula &b) {
  if (a.IsVar() != b.IsVar())
    return a.IsVar() > b.IsVar();
  if (a.IsVar())
    return a.Var() < b.Var();
  return a.hash < b.hash;
  // if (a.Op() != b.Op())
  //   return a.Op() < b.Op();
  // assert(a.Subformulas().size() == b.Subformulas().size());
  // for (int i = 0; i < a.Subformulas().size(); i++) {
  //   if (!(a.Subformula(i) == b.Subformula(i)))
  //     return a.Subformula(i) < b.Subformula(i);
  // }
  // return 0 < 0;
}
bool operator>(const Formula &a, const Formula &b) { return b < a; }

Formula FNot(const Formula &f) { return Formula(op_not, {f}); }
Formula FImpl(const Formula &a, const Formula &b) {
  return Formula(op_impl, {a, b});
}
Formula FId(const Formula &a, const Formula &b) {
  return Formula(op_id, {a, b});
}

Set::Set(const vector<Formula> &formulas) : formulas(formulas) {
  this->Normalize();
}
Set::Set(string input) {
  vector<string> input_formulas = SplitString(input, ",");
  for (auto s : input_formulas) {
    formulas.push_back(Formula(s));
  }
  this->Normalize();
}

const vector<Formula> &Set::Formulas() const { return formulas; }

void Set::Normalize() {
  if (formulas.size() > 1) {
    sort(formulas.begin(), formulas.end());
    auto last = unique(formulas.begin(), formulas.end());
    formulas.erase(last, formulas.end());
  }

  hash = 167;
  for (const auto &f : formulas) {
    hash += f.Hash();
    hash *= 13;
  }
}

void Set::ReplaceFormula(int index, Formula f) {
  assert(formulas.size() > index);
  formulas[index] = f;
  Normalize();
}

void Set::RemoveFormula(int index, bool normalize) {
  assert(formulas.size() > index);
  swap(formulas[index], formulas[formulas.size() - 1]);
  formulas.pop_back();
  if (normalize)
    Normalize();
}

void Set::AddFormula(const Formula &f, bool normalize) {
  formulas.push_back(f);
  if (normalize)
    Normalize();
}

vector<Set> ParseSets(string input) {
  vector<Set> result;
  auto strings = SplitString(input, "}");
  for (auto s : strings) {
    result.push_back(Set(s));
  }
  return result;
}

bool operator==(const Set &a, const Set &b) {
  return a.hash == b.hash;
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

bool operator<(const Set &a, const Set &b) {
  if (a.Formulas().size() != b.Formulas().size()) {
    return a.Formulas().size() < b.Formulas().size();
  }
  return a.hash < b.hash;
  for (int i = 0; i < a.Formulas().size(); i++) {
    if (a.Formulas()[i] != b.Formulas()[i]) {
      return a.Formulas()[i] < b.Formulas()[i];
    }
  }
  return 0 < 0;
}

void PrintProofNode(const ProofNode &n, string prefix) {
  cout << prefix << "--: " << n.root << "\t(is ";
  if (n.is_closed.has_value()) {
    cout << (n.is_closed.value() ? "closed" : "open");
  } else {
    cout << "?";
  }
  cout << ")";
  if (n.formula_used.has_value()) {
    cout << " applied rule " << n.rule_used.value() << " to "
         << n.formula_used.value();
  }
  cout << endl;
  for (int i = 0; i < n.subnodes.size(); i++) {
    string add_to_prefix = ((i + 1 < n.subnodes.size()) ? " |" : " |");
    PrintProofNode(n.subnodes[i], prefix + add_to_prefix);
  }
}

bool operator<(const ProofNode &a, const ProofNode &b) {
  return a.root < b.root;
}

bool operator==(const ProofNode &a, const ProofNode &b) {
  return a.root == b.root;
}