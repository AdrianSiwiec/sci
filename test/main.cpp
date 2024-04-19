#include "commons.h"
#include "label_set.h"
#include "label_solver.h"
#include "solver.h"
#include "structs.h"

int main() {
  // Formula f("-(phi->psi)");
  // Formula g("phi->theta");
  // Formula h("--phi");

  // ProofNode n(Set({f, g, h}));
  // PrintProofNode(n);

  // cout << endl << endl;
  // DoSolve("--ϕ -> (-ψ -> ϑ)");

  // DoSolve("-((p=q)->(q=p))");
  // DoSolve("--(p=q)");
  // DoSolve("-((p=-q)->((q=-r)->(p=--r)))");
  // DoSolve("p,-p");
  // DoSolve("p,p=-r,q=-s,p=(q=p),p=(s=r)");
  // DoSolve("p<>q, -(p=q)");

  // DoSolve("((((q≡p)→(p→r))≡(p≡(p→(p↔p))))→(((r∧p)↔(p≡p))∨((p∧p)∨¬q)))");
  // DoSolve("-((((q≡p)→(p→r))≡(p≡(p→(p↔p))))→(((r∧p)↔(p≡p))∨((p∧p)∨¬q)))");

  // DoSolve(
  //     "-((((q = ((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p)
  //     "
  //     "<> (p = p)) | ((p & p) | -q)))) -> (((((q = p) -> (p -> r)) = ((p ->
  //     (p "
  //     "<> p)) = p)) -> (((r & p) <> (p = p)) | ((p & p) | -q)))-> r)) = "
  //     "((((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> (p
  //     = " "p)) | ((p & p) | -q)))-> (((((q = p) -> (p -> r)) = ((p -> (p <>
  //     p)) = " "p)) -> (((r & p) <> (p = p)) | ((p & p) | -q)))<> ((((q = p)
  //     -> (p -> " "r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> (p = p)) | ((p
  //     & p) | "
  //     "-q))))) = ((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r &
  //     p) "
  //     "<> (p = p)) | ((p & p) | -q))))) -> (((r & ((((q = p) -> (p -> r)) = "
  //     "((p -> (p <> p)) = p)) -> (((r & p) <> (p = p)) | ((p & p) | -q)))) <>
  //     "
  //     "(((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> (p =
  //     " "p)) | ((p & p) | -q)))= ((((q = p) -> (p -> r)) = ((p -> (p <> p)) =
  //     " "p)) -> (((r & p) <> (p = p)) | ((p & p) | -q))))) | ((((((q = p) ->
  //     (p "
  //     "-> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> (p = p)) | ((p & p) |
  //     "
  //     "-q)))& ((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p)
  //     <> "
  //     "(p = p)) | ((p & p) | -q)))) | -q)))");

  // cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // for (string line; getline(cin, line);) {
  //   DoSolve(line);
  //   cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // }

  // LabelSet ls;
  // ls.CreateLabel(true, Formula("phi->psi"));
  // ls.CreateLabel(false, Formula("phi"));
  // ls.CreateLabel(true, Formula("a"));
  // ls.CreateLabel(true, Formula("b"));
  // ls.CreateLabel(true, Formula("c"));
  // ls.MakeEqual(3, 2);
  // ls.MakeEqual(-1, 2);
  // ls.MakeEqual(1, 4);
  // ls.MakeNotEqual(1, 3);
  // ls.MakeNotEqual(-1, 2);
  // cout << ls << endl;

  LabelNode ln;
  ln.root.CreateLabel(false, Formula("-((p=q)->(q=p))"));
  LabelSolve(ln);
  PrintLabelNode(ln);
}