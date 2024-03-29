#include "commons.h"
#include "solver.h"
#include "structs.h"

void Example(string s) {
  Formula f(s);
  ProofNode n(vector<Formula>{f});
  IsClosed(n);
  PrintProofNode(n);
  cout << endl;
}

int main() {
  Formula f("-(phi->psi)");
  Formula g("phi->theta");
  Formula h("--phi");

  ProofNode n(Set({f, g, h}));
  PrintProofNode(n);

  cout << endl << endl;
  Example("--ϕ -> (-ψ -> ϑ)");

  Example("-((p=q)->(q=p))");
  Example("--(p=q)");
  Example("-((p=-q)->((q=-r)->(p=--r)))");

  while (true) {
    cout << endl << "Enter your formula: " << endl;
    string input;
    cin >> input;
    f = Formula(input);
    n = ProofNode(vector<Formula>{f});
    PrintProofNode(n);
  }
}