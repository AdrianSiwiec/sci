#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"
#include <queue>

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
    "(psi = chi) -> ((phi = psi) = (phi = chi)),"   // 8, 4
    // "((-phi = phi) -> phi),"
    // "((-phi = phi) -> -phi),"
    // "((phi = -phi) -> phi),"
    // "((phi = -phi) -> -phi),",
    ,
    0);
int vars_needed[] = {2, 3, 2, 1, 2, 2, 4, 4, 1, 3, 3, 2, 1, 1, 1, 1};
// int vars_needed[] = {1, 2, 2, 4, 4, 3, 3};

map<int, Formula> proven_nrs;
map<Formula, int> proven;

map<Formula, vector<int>> willProve;

// ------- Refinement section --------------

map<int, vector<int>> parents;
map<int, vector<int>> children;
map<int, int> tree_size;
map<int, int> tree_length;

int get_size(int n) {
  if (n < 0)
    return 0;
  return tree_size[n];
}
int get_length(int n) {
  if (n < 0)
    return 0;
  return tree_length[n];
}

// ------- Refinement section end --------------

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

// // "((-phi = phi) -> phi),"
// // "((-phi = phi) -> -phi),"
// // "((phi = -phi) -> phi),"
// // "((phi = -phi) -> -phi),",
// bool isAx12t15(const Formula &f) {
//   return f.IsOp(Operator::op_impl) && f.Subformula(0).IsOp(Operator::op_id);
// }
// bool isAx12(const Formula &f) {
//   return isAx12t15(f) && f.Subformula(0).Subformula(0).IsOp(Operator::op_not)
//   &&
//          f.Subformula(1) == f.Subformula(0).Subformula(1) &&
//          f.Subformula(1) == f.Subformula(0).Subformula(0).Subformula();
// }
// bool isAx13(const Formula &f) {
//   return isAx12t15(f) && f.Subformula(1).IsOp(Operator::op_not) &&
//          f.Subformula(0).Subformula(0).IsOp(Operator::op_not) &&
//          f.Subformula(1).Subformula() == f.Subformula(0).Subformula(1) &&
//          f.Subformula(1).Subformula() ==
//              f.Subformula(0).Subformula(0).Subformula();
// }
// bool isAx14(const Formula &f) {
//   return isAx12t15(f) && f.Subformula(0).Subformula(1).IsOp(Operator::op_not)
//   &&
//          f.Subformula(1) == f.Subformula(0).Subformula(1).Subformula() &&
//          f.Subformula(1) == f.Subformula(0).Subformula(0);
// }
// bool isAx15(const Formula &f) {
//   return isAx12t15(f) && f.Subformula(1).IsOp(Operator::op_not) &&
//          f.Subformula(0).Subformula(1).IsOp(Operator::op_not) &&
//          f.Subformula(1).Subformula() == f.Subformula(0).Subformula(0) &&
//          f.Subformula(1).Subformula() ==
//              f.Subformula(0).Subformula(1).Subformula();
// }

bool isKRZOnly(const Formula &f) {
  if (f.IsOp(Operator::op_id))
    return false;
  if (f.IsVar())
    return true;
  if (f.IsOp(Operator::op_not))
    return isKRZOnly(f.Subformula());
  return isKRZOnly(f.Subformula(0)) && isKRZOnly(f.Subformula(1));
}
// Only one variable (p)
bool isKRZTrue(const Formula &f, bool val) {
  if (f.IsVar())
    return val;
  if (f.IsOp(Operator::op_not))
    return !isKRZTrue(f.Subformula(), val);
  if (f.IsOp(Operator::op_impl))
    return !isKRZTrue(f.Subformula(0), val) || isKRZTrue(f.Subformula(1), val);
  assert("Checking KRZ true for unsupported operators" == 0);
  exit(1);
}

bool isKRZTautology(const Formula &f) {
  if (!isKRZOnly(f))
    return false;
  if (!isKRZTrue(f, true))
    return false;
  if (!isKRZTrue(f, false))
    return false;
  return true;
}

void AddProvenFormula(const Formula &f, int a, int b, bool tryNewProofs) {
  if (proven.count(f) > 0)
    return;

  if (isAx1(f) || isAx2(f) || isAx3(f) || isAx4(f) || isAx5(f) || isAx6(f) ||
      isAx7(f) || isAx8(f) || isAx9(f) || isAx10(f) ||
      isAx11(f)
      // || isAx12(f) || isAx13(f) || isAx14(f) || isAx15(f)
      || isKRZTautology(f))
    return;

  // cout << a << ";" << b << ";" << f << endl;
  // if (tryNewProofs) {
  //   cerr << a << ";" << b << ";" << f << endl;
  // }

  int i = proven.size() + 1;
  proven_nrs.emplace(i, f);
  proven[f] = i;

  parents[i] = {a, b};
  tree_size[i] = get_size(a) + get_size(b) + 1;
  tree_length[i] = max(get_length(a), get_length(b)) + 1;

  if (a > 0) {
    if (children.count(a) == 0)
      children[a] = {};
    children[a].push_back(i);
  }
  if (b > 0) {
    if (children.count(b) == 0)
      children[b] = {};
    children[b].push_back(i);
  }

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
      // if (isAx12(f.Subformula(0))) {
      //   AddProvenFormula(f.Subformula(1), -axiom - 1, -12, true);
      // }
      // if (isAx13(f.Subformula(0))) {
      //   AddProvenFormula(f.Subformula(1), -axiom - 1, -13, true);
      // }
      // if (isAx14(f.Subformula(0))) {
      //   AddProvenFormula(f.Subformula(1), -axiom - 1, -14, true);
      // }
      // if (isAx15(f.Subformula(0))) {
      //   AddProvenFormula(f.Subformula(1), -axiom - 1, -15, true);
      // }
      if (isKRZTautology(f.Subformula(0))) {
        AddProvenFormula(f.Subformula(1), -axiom - 1, -99, true);
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

// vector<Formula> interesting = DoParseFormulas(
//     "((p≡¬p)→((p≡p)≡(p≡¬p))),((¬p≡p)"
//     "→((p≡¬p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡¬p))),"
//     "((p≡¬p)→((p≡¬p)≡(¬p≡¬p))),((p≡¬p)→((p→p)≡(p→¬p))),((¬p≡p)→((p→¬p)≡(p→p))),"
//     "((¬p≡p)→((¬p≡p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡p))),((p≡¬p)→((p≡p)≡(¬p≡p))),("
//     "(p≡¬p)→((p≡p)≡(¬p≡¬p))),((¬p≡p)→((¬p→¬p)≡(p→¬p))),((¬p≡p)→((¬p→¬p)≡(p→p)))"
//     ",((p≡¬p)→((p→p)≡(¬p→¬p)))",
//     0);

void tryAddRandomAxiom() {
  // Only axioms 1, 2, 3 make sense as major of MP
  int axiom = rand() % (axioms.size());
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
    values.push_back(GetRandomFormula((rand() % maxsize) + 1, 1));
  }
  TryAxiom(axiom + 1, values);
}

// --------------- refinement section -----------

map<int, int> parent_count;

void calculate_tree_sizes(int n) {
  // tree_size[n] = 1;
  // tree_length[n] = 1;
  parent_count[n] = 0;
  for (int parent : parents[n]) {
    if (parent > 0) {
      calculate_tree_sizes(parent);
      // tree_size[n] += tree_size[parent];
      // tree_length[n] = max(tree_length[n], tree_length[parent] + 1);
      parent_count[n]++;
    }
  }
}

void print_tree(int n) {
  parent_count.clear();
  calculate_tree_sizes(n);

  map<int, Formula> new_nrs;
  map<Formula, int> new_formulas;

  queue<int> leafs;
  for (auto &p : parent_count) {
    if (p.second == 0) {
      leafs.push(p.first);
    }
  }

  while (!leafs.empty()) {
    int old_nr = leafs.front();
    leafs.pop();

    Formula f = proven_nrs.at(old_nr);
    int nr = new_formulas.size() + 1;
    new_formulas[f] = nr;
    new_nrs.emplace(nr, f);

    for (int parent : parents[old_nr]) {
      if (parent < 0) {
        cout << parent;
      } else {
        auto parent_formula = proven_nrs.at(parent);
        cout << new_formulas[parent_formula];
      }
      cout << ";";
    }
    cout << f << endl;

    for (int child : children[old_nr]) {
      parent_count[child]--;
      if (parent_count[child] == 0) {
        leafs.push(child);
      }
    }
  }

  cout << "# Naive Size: " << tree_size[n] << endl;
  cout << "# Real Size: " << parent_count.size() << endl;
  cout << "# Length: " << tree_length[n] << endl;
}

int main() {
  std::ios::sync_with_stdio(false);

  srand(time(0));
  for (string line; getline(cin, line);) {
    if (line.size() > 0 && line[0] == '#') {
      continue;
    }
    // cout << line << endl;
    vector<string> input = SplitString(line, ";");
    Formula f = DoParseFormulas(input[2], 0)[0];
    AddProvenFormula(f, stoi(input[0]), stoi(input[1]), false);
  }

  Formula target = Formula("-(p=-p)");

  int target_nr = proven[target];
  calculate_tree_sizes(target_nr);

  print_tree(target_nr);

  // Formula targe2 = Formula("-(-p=p)");
  // while (true) {
  //   tryAddRandomAxiom();
  //   if (proven.count(target) > 0 || proven.count(targe2) > 0) {
  //     cout << "FOUND " << target << " as " << proven[target] << endl;
  //     exit(0);
  //   }
  // }
}