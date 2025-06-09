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

  // assert(DoSolve("¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), (b≡¬c)").is_closed.value());
  // assert(
  //     DoSolve(
  //         "¬((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), (b≡¬c), a, "
  //         "-((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), -(a→((d≡¬c)≡(b≡d))),
  //         -((d≡¬c)≡(b≡d))") .is_closed.value());
  // assert(DoSolve("¬((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), (b≡¬c), a, b, ¬c, "
  //                "-((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), -(a→((d≡¬c)≡(b≡d))), "
  //                "-((d≡¬c)≡(b≡d)), -c, -(d≡¬c), -(b≡d)")
  //            .is_closed.value());
  // assert(DoSolve("¬((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), (b≡¬c), a, b, ¬c, d, ¬c, b,
  // d, "
  //                "-((b≡¬c)→(a→((d≡¬c)≡(b≡d)))), -(a→((d≡¬c)≡(b≡d))), "
  //                "-((d≡¬c)≡(b≡d)), -c, -(d≡¬c), -(b≡d), -c")
  //            .is_closed.value());

  // LabelNode l;
  // // 1:b, 2:¬c, 3:d, 4:¬c, -1:(d≡¬c), -2:(b≡d), -3:c}	Equalities:
  // 1=2=4; Non-equalities: -2≠-1, 3≠4; l.root.CreateLabel(true, Formula("b"));
  // l.root.CreateLabel(true, Formula("-c"));
  // l.root.CreateLabel(true, Formula("d"));
  // l.root.CreateLabel(true, Formula("-c"));
  // l.root.CreateLabel(false, Formula("d=-c"));
  // l.root.CreateLabel(false, Formula("b=d"));
  // l.root.CreateLabel(false, Formula("c"));
  // l.root.MakeEqual(1,2);
  // l.root.MakeEqual(2,4);
  // l.root.MakeNotEqual(-2,-1);
  // l.root.MakeNotEqual(3,4);
  // LabelSolve(l);
  // PrintLabelNode(l);
  // assert(l.is_closed.value());

  // Formula("b");
  // assert(DoSolve("b,-c,d=-c,d=b,-((d=-c)=(b=d)),b=-c",true).is_closed.value());
  // LabelNode ln;
  // ln.root.CreateLabel(true, Formula("b"));
  // ln.root.CreateLabel(true, Formula("-c"));
  // ln.root.CreateLabel(false, Formula("d=-c"));
  // ln.root.CreateLabel(false, Formula("d=b"));
  // ln.root.CreateLabel(false, Formula("c"));
  // ln.used_rules.insert(make_pair(2, LRNotPlus));
  // ln.root.MakeNotEqual(-1,-2);
  // ln.root.MakeEqual(1,2);
  // LabelSolve(ln);
  // PrintLabelNode(ln);
  // assert(ln.is_closed.value());

  TestInput("(((c≡a)≡d)≡(d→a))", true);
  TestInput("-((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <> "
            "(p = p)) | ((p & p) | -q)))");

  // cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // for (string line; getline(cin, line);) {
  //   DoSolve(line);
  //   cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // }

  // cout << "LABEL SOLVE" << endl;
  // for (string line; getline(cin, line);) {
  //   DoSolveLabel(line, true);
  //   cout << endl << "Enter your set of formulas (divided by ','): " << endl;
  // }
}