#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"

auto formula = DoParseFormulas("p", 0)[0];
auto vars = DoParseFormulas("phi, psi, chi, theta", 0);
auto axioms = DoParseFormulas(
    "phi -> (psi -> phi),"
    "(phi -> (psi -> chi)) -> ((phi -> psi) -> (phi -> chi)),"
    "(-phi -> -psi) -> (psi -> phi),"
    "phi = phi,"
    "(phi = psi) -> (phi -> psi),"
    "(phi = psi) -> (-phi = -psi),"
    "(phi = psi) -> ((chi = theta) -> ((phi -> chi) = (psi -> theta))),"
    "(phi = psi) -> ((chi = theta) -> ((phi = chi) = (psi = theta))),"
    "phi -> phi,"
    "(psi = chi) -> ((phi -> psi) = (phi -> chi))," // 7, 4
    "(psi = chi) -> ((phi = psi) = (phi = chi))",   // 8, 4
    0);
int vars_needed[] = {2, 3, 2, 1, 2, 2, 4, 4, 1, 3, 3, 2};

map<int, Formula> proven_nrs;
map<Formula, int> proven;

map<Formula, vector<int>> willProve;

bool isAx1(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(0) == f.Subformula(1).Subformula(1);
}
bool isAx2(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(0).Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(1).Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(0).Subformula(0) ==
             f.Subformula(1).Subformula(0).Subformula(0) &&
         f.Subformula(0).Subformula(0) ==
             f.Subformula(1).Subformula(1).Subformula(0) &&
         f.Subformula(0).Subformula(1).Subformula(0) ==
             f.Subformula(1).Subformula(0).Subformula(1) &&
         f.Subformula(0).Subformula(1).Subformula(1) ==
             f.Subformula(1).Subformula(1).Subformula(1);
}
bool isAx3(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(0).Subformula(0).IsOp(Operator::op_not) &&
         f.Subformula(0).Subformula(0).Subformula() ==
             f.Subformula(1).Subformula(1) &&
         f.Subformula(0).Subformula(1).IsOp(Operator::op_not) &&
         f.Subformula(0).Subformula(1).Subformula() ==
             f.Subformula(1).Subformula(0);
}
bool isAx4(const Formula &f) {
  return f.IsOp(Operator::op_id) && f.Subformula(0) == f.Subformula(1);
}
bool isAx5(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).IsOp(Operator::op_impl) &&
         f.Subformula(0).Subformula(0) == f.Subformula(1).Subformula(0) &&
         f.Subformula(0).Subformula(1) == f.Subformula(1).Subformula(1);
}
// "(phi = psi) -> (-phi = -psi),"
bool isAx6(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).IsOp(Operator::op_id) &&
         f.Subformula(1).Subformula(0).IsOp(Operator::op_not) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_not) &&
         f.Subformula(1).Subformula(0).Subformula() ==
             f.Subformula(0).Subformula(0) &&
         f.Subformula(1).Subformula(1).Subformula() ==
             f.Subformula(0).Subformula(1);
}
bool isAx7or8a(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(0).Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(0).Subformula(1).IsOp(Operator::op_id) &&
         f.Subformula(1).IsOp(Operator::op_id);
}
bool isAx7or8b(const Formula &f) {
  return f.Subformula(0).Subformula(0).Subformula(0) ==
             f.Subformula(1).Subformula(0).Subformula(0) &&
         f.Subformula(0).Subformula(0).Subformula(1) ==
             f.Subformula(1).Subformula(1).Subformula(0) &&
         f.Subformula(0).Subformula(1).Subformula(0) ==
             f.Subformula(1).Subformula(0).Subformula(1) &&
         f.Subformula(0).Subformula(1).Subformula(1) ==
             f.Subformula(1).Subformula(1).Subformula(1);
}
// "(phi = psi) -> ((chi = theta) -> ((phi -> chi) = (psi -> theta))),"
bool isAx7(const Formula &f) {
  return isAx7or8a(f) &&
         f.Subformula(1).Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_impl) && isAx7or8b(f);
}
// "(phi = psi) -> ((chi = theta) -> ((phi = chi) = (psi = theta)))",
bool isAx8(const Formula &f) {
  return isAx7or8a(f) && f.Subformula(1).Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_id) && isAx7or8b(f);
}
bool isAx9(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0) == f.Subformula(1);
}
bool isAx10or11(const Formula &f) {
  return f.Subformula(0).Subformula(0) ==
             f.Subformula(1).Subformula(0).Subformula(1) &&
         f.Subformula(0).Subformula(1) ==
             f.Subformula(1).Subformula(1).Subformula(1) &&
         f.Subformula(1).Subformula(0).Subformula(0) ==
             f.Subformula(1).Subformula(1).Subformula(0);
}
// "(psi = chi) -> ((phi -> psi) = (phi -> chi)),"
bool isAx10(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).IsOp(Operator::op_id) &&
         f.Subformula(1).Subformula(0).IsOp(Operator::op_impl) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_impl) && isAx10or11(f);
}
// "(psi = chi) -> ((phi = psi) = (phi = chi))",
bool isAx11(const Formula &f) {
  return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).IsOp(Operator::op_id) &&
         f.Subformula(1).Subformula(0).IsOp(Operator::op_id) &&
         f.Subformula(1).Subformula(1).IsOp(Operator::op_id) && isAx10or11(f);
}

void AddProvenFormula(const Formula &f, int a, int b, bool tryNewProofs) {
  if (proven.count(f) > 0)
    return;

  if (isAx1(f) || isAx2(f) || isAx3(f) || isAx4(f) || isAx5(f) || isAx6(f) ||
      isAx7(f) || isAx8(f) || isAx9(f) || isAx10(f) || isAx11(f))
    return;

  cout << a << ";" << b << ";" << f << endl;
  if (tryNewProofs) {
    cerr << a << ";" << b << ";" << f << endl;
  }

  int i = proven.size() + 1;
  proven_nrs.emplace(i, f);
  proven[f] = i;

  if (f.IsOp(Operator::op_impl)) {
    if (proven.count(f.Subformula(0)) > 0) {
      if (tryNewProofs) {
        AddProvenFormula(f.Subformula(1), i, proven[f.Subformula(0)], true);
      }
    } else {
      willProve[f.Subformula(0)].push_back(i);
    }
  }

  if (tryNewProofs) {
    if (willProve.count(f) > 0) {
      const auto &nrs = willProve[f];
      for (int nr : nrs) {
        AddProvenFormula(proven_nrs.at(nr).Subformula(1), nr, i, true);
      }
      willProve.erase(f);
    }
  }
}

void TryAxiom(int axiom, vector<Formula> values) {
  axiom--;
  assert(values.size() + 1 >= vars_needed[axiom]);
  Formula f = axioms[axiom];
  for (int i = 0; i < vars_needed[axiom]; i++) {
    ReplaceAll(f, vars[i], values[i]);
  }

  if (f.IsOp(Operator::op_impl)) {
    if (proven.count(f.Subformula(1)) == 0) {
      if (proven.count(f.Subformula(0)) > 0) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, proven[f.Subformula(0)],
                         true);
      }
      if (isAx1(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -1, true);
      }
      if (isAx2(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -2, true);
      }
      if (isAx3(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -3, true);
      }
      if (isAx4(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -4, true);
      }
      if (isAx5(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -5, true);
      }
      if (isAx6(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -6, true);
      }
      if (isAx7(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -7, true);
      }
      if (isAx8(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -8, true);
      }
      if (isAx9(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -9, true);
      }
      if (isAx10(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -10, true);
      }
      if (isAx11(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -11, true);
      }
    }
  }

  if (willProve.count(f) > 0) {
    const auto nrs = willProve[f];
    for (int nr : nrs) {
      AddProvenFormula(proven_nrs.at(nr).Subformula(1), nr, -axiom - 1, true);
    }
    willProve.erase(f);
  }
}

vector<Formula> interesting = DoParseFormulas(
    "((p≡¬p)→((p≡p)≡(p≡¬p))),((¬p≡p)"
    "→((p≡¬p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡¬p))),"
    "((p≡¬p)→((p≡¬p)≡(¬p≡¬p))),((p≡¬p)→((p→p)≡(p→¬p))),((¬p≡p)→((p→¬p)≡(p→p))),"
    "((¬p≡p)→((¬p≡p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡p))),((p≡¬p)→((p≡p)≡(¬p≡p))),("
    "(p≡¬p)→((p≡p)≡(¬p≡¬p))),((¬p≡p)→((¬p→¬p)≡(p→¬p))),((¬p≡p)→((¬p→¬p)≡(p→p)))"
    ",((p≡¬p)→((p→p)≡(¬p→¬p)))",
    0);

void tryAddRandomAxiom() {
  // Only axioms 1, 2, 3 make sense as major of MP
  int axiom = rand() % 3;
  vector<Formula> values;
  int maxsize = 3;
  maxsize = 3;
  if ((rand() % 4) == 0) {
    maxsize = 4;
    if ((rand() % 8) == 0) {
      maxsize = 5;
      if ((rand() % 8) == 0) {
        maxsize = 6;
        if ((rand() % 8) == 0) {
          maxsize = 7;
        }
      }
    }
  }
  while (values.size() < vars_needed[axiom]) {
    values.push_back(GetRandomFormula((rand() % maxsize) + 1, 1, interesting));
  }
  TryAxiom(axiom + 1, values);
}

int main() {
  std::ios::sync_with_stdio(false);

  srand(time(0));
  for (string line; getline(cin, line);) {
    // cout << line << endl;
    vector<string> input = SplitString(line, ";");
    Formula f = DoParseFormulas(input[2], 0)[0];
    AddProvenFormula(f, stoi(input[0]), stoi(input[1]), false);
  }

  cerr << "Read done, read " << proven.size() << " formulas." << endl;

  Formula target = Formula("-(p=-p)");
  Formula targe2 = Formula("-(-p=p)");
  while (true) {
    tryAddRandomAxiom();
    if (proven.count(target) > 0 || proven.count(targe2) > 0) {
      cout << "FOUND " << target << " as " << proven[target] << endl;
      exit(0);
    }
  }
}
