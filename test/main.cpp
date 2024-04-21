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
  //     "-((((((((q=p)->(p->r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))->("
  //     "((((q=p)->(p->r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))<>((((q="
  //     "p)->(p->r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))))=((((q=p)->("
  //     "p->r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q))))=((q=((((q=p)->(p-"
  //     ">r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q))))->(((((q=p)->(p->r))"
  //     "=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))->r)))->(((r&((((q=p)->("
  //     "p->r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q))))<>(((((q=p)->(p->"
  //     "r))=(p=(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))=((((q=p)->(p->r))=(p="
  //     "(p->(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))))|((((((q=p)->(p->r))=(p=(p-"
  //     ">(p<>p))))->(((r&p)<>(p=p))|((p&p)|-q)))&((((q=p)->(p->r))=(p=(p->(p<>p)"
  //     ")))->(((r&p)<>(p=p))|((p&p)|-q))))|-q)))");

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

  // LabelNode ln;
  // ln.root.CreateLabel(false, Formula("-((p=q)->(q=p))"));
  // LabelSolve(ln);
  // PrintLabelNode(ln);

  // LabelNode ln;
  // ln.root.CreateLabel(
  //     true,
  //     Formula("-((((q≡p)→(p→r))≡(p≡(p→(p↔p))))→(((r∧p)↔(p≡p))∨((p∧p)∨¬q)))"));
  // LabelSolve(ln);
  // PrintLabelNode(ln);

  // for (string line; getline(cin, line);) {
  //   DoSolveLabel(line, true);
  //   cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // }

  // ProofNode pn = DoSolve("¬(((b→b)→b)→b)", true);
  // assert(pn.is_closed.value());

  // DoSolve("b, ¬v1, (b≡¬v1), (v2≡¬b), (v1≡¬v2)", true);
  // assert(DoSolve("b, ¬v1, (b≡¬v1), (v2≡¬b), (v1≡¬v2)",
  // true).is_closed.value()); assert(DoSolve("b, ¬v1, (b≡¬v1), (v2≡¬b),
  // (v1≡¬v2)", true).is_closed.value());

  // assert(DoSolve("p, (p≡¬q), (r≡¬p), (q≡¬r)", true).is_closed.value());
  // assert(DoSolve("p, ¬q, (p≡¬q), (r≡¬p), (q≡¬r)", true).is_closed.value());
  // assert(DoSolve("b≡¬¬¬b", true).is_closed.value());
  // assert(pn.is_closed.value());
  Formula("b");
  DoSolveLabel("¬(¬(a≡(b≡a))→(a→(b→a)))", true);
  assert(DoSolve("¬(¬(a≡(b≡a))→(a→(b→a)))", true).is_closed.value());


  cout << "LABEL SOLVE" << endl;
  for (string line; getline(cin, line);) {
    DoSolveLabel(line, true);
    cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  }
}