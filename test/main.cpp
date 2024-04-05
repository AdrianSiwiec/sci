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
  DoSolve("--ϕ -> (-ψ -> ϑ)");

  DoSolve("-((p=q)->(q=p))");
  DoSolve("--(p=q)");
  DoSolve("-((p=-q)->((q=-r)->(p=--r)))");
  DoSolve("p,-p");
  DoSolve("p,p=-r,q=-s,p=(q=p),p=(s=r)");
  DoSolve("p<>q, -(p=q)");
  DoSolve("((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> "
          "(p = p)) | ((p & p) | -q)))");

  // cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // for (string line; getline(cin, line);) {
  //   DoSolve(line);
  //   cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // }
}