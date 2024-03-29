#include "commons.h"
#include "solver.h"
#include "structs.h"

void Example(string s) {
  Set input_set(s);
  ProofNode n(input_set);
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
  Example("p,-p");
  Example("p,p=-r,q=-s,p=(q=p),p=(s=r)");


  while (true) {
    cout << endl << "Enter your set of formulas (divided by ','): " << endl;
    string input;
    cin >> input;
    Set s(input);
    n = ProofNode(s);
    Solve(n);
    PrintProofNode(n);
  }
}