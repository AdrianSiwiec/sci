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
    "(phi = psi) -> ((chi = theta) -> ((phi = chi) = (psi = theta)))",
    0);
int vars_needed[] = {2, 3, 2, 1, 2, 2, 4, 4};
set<Formula> proven;
map<Formula, int> nrs;
vector<Formula> toMP;

bool IsTrivial(const Formula &f) {
  if (f.IsOp(Operator::op_id) || f.IsOp(Operator::op_impl)) {
    if (f.Subformula(0) == f.Subformula(1))
      return true;
  }
  if (f.IsOp(Operator::op_impl) && f.Subformula(1).IsOp(Operator::op_impl) &&
      f.Subformula(0) == f.Subformula(1).Subformula(1))
    return true;
  if (f.IsOp(Operator::op_impl) && IsTrivial(f.Subformula(1)))
    return true;
  return false;
}

bool AddAxiom(int axiom, vector<Formula> values,
              bool interesting_only = false) {
  axiom--;
  assert(values.size() + 1 >= vars_needed[axiom]);
  Formula f = axioms[axiom];
  for (int i = 0; i < vars_needed[axiom]; i++) {
    ReplaceAll(f, vars[i], values[i]);
  }

  if (proven.count(f) == 0) {
    if (interesting_only) {
      if (!f.IsOp(Operator::op_impl)) {
        return false;
      }
      Formula pre = f.Subformula(0);
      Formula post = f.Subformula(1);
      if (!(proven.count(pre) != proven.count(post))) {
        return false;
      }

      if (IsTrivial(f))
        return false;
      if ((axiom == 6 || axiom == 7) &&
          f.Subformula(0).Subformula(0) == f.Subformula(0).Subformula(1)) {
        return false;
      }
    }
    // cerr << "Adding " << f << " as an axiom " << axiom + 1 << endl;
    cout << "0;" << axiom + 1 << ";" << f << endl;
    proven.insert(f);
    nrs[f] = proven.size();
    toMP.push_back(f);
    return true;
  }
  return false;
}

bool tryMP() {
  if (!toMP.empty()) {
    Formula f = toMP.back();
    if (f.IsOp(Operator::op_impl)) {
      const auto pre = f.Subformula(0);
      const auto post = f.Subformula(1);
      if (proven.count(pre) > 0 && proven.count(post) == 0) {
        int nr = nrs[f];
        int pre_nr = nrs[pre];
        cerr << "Adding " << post << " as MP from " << nr << ", " << pre_nr
             << endl;
        cout << nr << ";" << pre_nr << ";" << post << endl;
        toMP.pop_back();

        proven.insert(post);
        nrs[post] = proven.size();
        toMP.push_back(post);
        return true;
      }
    } else {
      toMP.pop_back();
      return true;
    }
  }
  return false;
}

vector<Formula> interesting = DoParseFormulas(
    "((p≡¬p)→((p≡p)≡(p≡¬p))),((¬p≡p)"
    "→((p≡¬p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡¬p))),"
    "((p≡¬p)→((p≡¬p)≡(¬p≡¬p))),((p≡¬p)→((p→p)≡(p→¬p))),((¬p≡p)→((p→¬p)≡(p→p))),"
    "((¬p≡p)→((¬p≡p)≡(p≡p))),((¬p≡p)→((¬p≡¬p)≡(p≡p))),((p≡¬p)→((p≡p)≡(¬p≡p))),("
    "(p≡¬p)→((p≡p)≡(¬p≡¬p))),((¬p≡p)→((¬p→¬p)≡(p→¬p))),((¬p≡p)→((¬p→¬p)≡(p→p)))"
    ",((p≡¬p)→((p→p)≡(¬p→¬p)))",
    0);

bool tryAddRandomAxiom() {
  int axiom = rand() % 8;
  // int axiom = 1;
  vector<Formula> values;
  while (values.size() < vars_needed[axiom]) {
    // values.push_back(GetRandomFormula((rand()% 3) + 1, 1));
    values.push_back(GetRandomFormula((rand() % 6) + 1, 1, interesting));
  }
  return AddAxiom(axiom + 1, values, /*interesting=*/true);
}

int main() {
  std::ios::sync_with_stdio(false);
  // // // Kickstart
  // AddAxiom(1, {Formula("p"), Formula("p")});
  // AddAxiom(2, {Formula("p"), Formula("p"), Formula("p")});
  // AddAxiom(3, {Formula("p"), Formula("p")});
  // AddAxiom(4, {Formula("p")});
  // AddAxiom(5, {Formula("p"), Formula("p"), Formula("p")});
  // AddAxiom(6, {Formula("p"), Formula("p"), Formula("p")});
  // AddAxiom(7, {Formula("p"), Formula("p"), Formula("p"), Formula("p")});
  // AddAxiom(8, {Formula("p"), Formula("p"), Formula("p"), Formula("p")});
  // return 0;

  srand(time(0));
  for (string line; getline(cin, line);) {
    cout << line << endl;
    vector<string> input = SplitString(line, ";");
    Formula f = DoParseFormulas(input[2], 0)[0];
    proven.insert(f);
    nrs[f] = proven.size();
    toMP.push_back(f);
    // cerr << "Read " << f << " as " << nrs[f] << endl;
  }

  cerr << "Read done, read " << proven.size() << " formulas." << endl;

  Formula target = Formula("-(p=-p)");
  Formula targe2 = Formula("-(-p=p)");
  while (true) {
    bool added = tryAddRandomAxiom();
    if (added) {
      while (tryMP())
        ;
      if (proven.count(target) > 0 || proven.count(targe2) > 0) {
        cout << "FOUND " << target << " as " << nrs[target] << endl;
      }
    }
  }
}
