#include "commons.h"
#include "solver.h"
#include "structs.h"

int main() {
  Formula f("-(phi->psi)");
  Formula g("phi->theta");
  Formula h("--phi");

  ProofNode n(Set({f, g, h}));
  PrintProofNode(n);

  cout << endl << endl;

  f = Formula("--ϕ -> (-ψ -> ϑ)");
  n = ProofNode(vector<Formula>{f});
  PrintProofNode(n);

  while (true) {
    cout << endl << "Enter your formula: " << endl;
    string input;
    cin >> input;
    f = Formula(input);
    n = ProofNode(vector<Formula>{f});
    PrintProofNode(n);
  }
}