#include "label_rules.h"
#include "label_solver.h"

void TestApplyRule() {
  LabelNode n;
  n.root.CreateLabel(true, Formula("-phi"));
  n.root.CreateLabel(false, Formula("-phi"));
  auto result = ApplyRule(n, LRNotPlus, true);
  assert(result.size() == 1);
  assert(result[0].root.GetLabels().size() == 3);
  assert(result[0].root.GetFormula(-2) == Formula("phi"));
  assert(result[0].used_rules.size() == 1);
  assert(result[0].used_rules.count(make_pair(1, LRNotPlus)) == 1);
}

void TestSolveTermination() {
  LabelNode n;
  LabelSolve(n);
  assert(n.is_closed.value() == false);

  n.root.CreateLabel(true, Formula("phi"));
  n.root.CreateLabel(true, Formula("psi"));
  n.root.MakeEqual(1, 2);
  n.root.MakeNotEqual(2, 1);
  LabelSolve(n);
  assert(n.is_closed.value() == true);

  n = LabelNode();
  n.root.CreateLabel(true, Formula("phi"));
  n.root.CreateLabel(false, Formula("psi"));
  n.root.MakeEqual(1, -1);
  LabelSolve(n);
  assert(n.is_closed.value() == true);
}

void TestCompare() {
  TestInput("p");
  TestInput("p,q, -(p=q)");
  TestInput("p,q,-p,-(p=q)");
  TestInput(
      "v12, v10, v9, v6, v4, v3, r, q, -p, -v8, -v11, (p=p), (v4=v3), "
      "(v8=-v9), (v11=-v10), (v11=-v12), (v6=(p->r)), (v12=(p->p)), "
      "(v7=(p->v8)), (v5=(p=q)), (v4=(v5->v6)), (v4=(v7=p)), (v9=(v10->v11))");
  TestInput(
      "q, ¬p, ¬v3, ¬v4,(v5≡(q≡p)), v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput("q, v8, ¬p, ¬v3, ¬v4, ¬v9, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), "
            "(v12≡(p→p)), (v7≡(p→v8)), (v6≡(p→r)), (v5≡(q≡p)), (v3≡(p≡v7)), "
            "(v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput(
      "q, ¬p, (p≡p), (v3≡v4), (v8≡¬v9), (v11≡¬v12), (v6≡(p→r)), (v12≡(p→p)), "
      "(v7≡(p→v8)), (v5≡(p≡q)), (v3≡(p≡v7)), (v3≡(v5→v6)), (v9≡(v10→v11))");
  TestInput("-((((q = p) -> (p -> r)) = ((p -> (p <> p)) = p)) -> (((r & p) <>"
            "(p = p)) | ((p & p) | -q)))");
  TestInput("¬(((b→b)→b)→b)");
  TestInput("b≡((b↔b)∧¬b)");
  TestInput("a≡¬¬(a→¬a)");
  // TestInput("a≡¬((b↔a)↔b)");

  TestInput("¬((b≡¬c)→(a→((b≡d)≡(d≡¬c))))");
//   --: {¬((b≡¬c)→(a→((b≡d)≡(d≡¬c))))}	(is closed) applied rule ¬→ to ¬((b≡¬c)→(a→((b≡d)≡(d≡¬c))))
//  |--: {(b≡¬c), ¬(a→((b≡d)≡(d≡¬c)))}	(is closed) applied rule ¬→ to ¬(a→((b≡d)≡(d≡¬c)))
//  | |--: {a, (b≡¬c), ¬((b≡d)≡(d≡¬c))}	(is closed) applied rule fun to (b≡¬c)
//  | | |--: {a, (b≡¬c), ¬((b≡d)≡(b≡d))}	(is closed)
// --: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c))))}	Equalities:;	Non-equalities:;
//  |		Applied rule ¬+ to 1 to generate descendant;
//  |		(is open)
//  |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c))))}	Equalities:;	Non-equalities:;
//  | |		Applied rule →- to -1 to generate descendant;
//  | |		(is open)
//  | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c)))}	Equalities:;	Non-equalities:;
//  | | |		Applied rule →- to -2 to generate descendant;
//  | | |		(is open)
//  | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c))}	Equalities:;	Non-equalities:;
//  | | | |		Applied rule ≡+ to 2 to generate descendants;
//  | | | |		(is open)
//  | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c))}	Equalities: 4=5;	Non-equalities:;
//  | | | | |		Applied rule ¬+ to 5 to generate descendant;
//  | | | | |		(is open)
//  | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c}	Equalities: 4=5;	Non-equalities:;
//  | | | | | |		Applied rule ≡- to -3 to generate descendants;
//  | | | | | |		(is open)
//  | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), 7:(d≡¬c), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c}	Equalities: 4=5;	Non-equalities: 6≠7;
//  | | | | | | |		Applied rule ≡+ to 6 to generate descendants;
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), 7:(d≡¬c), 8:b, 9:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c}	Equalities: 2=6=7, 4=5=8=9;	Non-equalities: 6≠7;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), 7:(d≡¬c), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:b, -6:d}	Equalities: -6=-5=4=5, 2=6=7;	Non-equalities: 6≠7;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(d≡¬c)}	Equalities: 4=5;	Non-equalities:;
//  | | | | | | |		Applied rule ≡+ to 6 to generate descendants;
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), 7:b, 8:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(d≡¬c)}	Equalities: -5=2=6, 4=5=7=8;	Non-equalities:;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(b≡d), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(d≡¬c), -6:b, -7:d}	Equalities: -7=-6=4=5, -5=2=6;	Non-equalities:;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(d≡¬c), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d)}	Equalities: 4=5;	Non-equalities:;
//  | | | | | | |		Applied rule ≡+ to 6 to generate descendants;
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(d≡¬c), 7:d, 8:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d)}	Equalities: -5=2=6, 4=5=7=8;	Non-equalities:;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:(d≡¬c), -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:d, -7:¬c}	Equalities: -7=-6=4=5, -5=2=6;	Non-equalities:;
//  | | | | | | |		Equality rules used: [F ,≡≡ ,≡≡]
//  | | | | | | |		(is closed)
//  | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c)}	Equalities: 4=5;	Non-equalities: -6≠-5;
//  | | | | | | |		Applied rule ≡- to -5 to generate descendants;
//  | | | | | | |		(is open)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c)}	Equalities: 4=5=6;	Non-equalities: -6≠-5, 6≠7;
//  | | | | | | | |		Equality rules used: [F]
//  | | | | | | | |		Applied rule ≡- to -6 to generate descendants;
//  | | | | | | | |		(is open)
//  | | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, 8:d, 9:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c)}	Equalities: 4=5=6=9, 7=8;	Non-equalities: -6≠-5, 6≠7, 8≠9;
//  | | | | | | | | |		Equality rules used: [F ,F ,F]
//  | | | | | | | | |		Applied rule ¬+ to 9 to generate descendant;
//  | | | | | | | | |		(is open)
//  | | | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, 8:d, 9:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:c}	Equalities: -7=-4, 4=5=6=9, 7=8;	Non-equalities: -6≠-5, 6≠7, 8≠9;
//  | | | | | | | | |		Equality rules used: [F ,F ,F ,F]
//  | | | | | | | | |		(is open)
//  | | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, 8:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:¬c}	Equalities: 4=5=6;	Non-equalities: -6≠-5, 6≠7;
//  | | | | | | | |		Equality rules used: [F]
//  | | | | | | | |		(is ?)
//  | | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, 8:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:d}	Equalities: 4=5=6;	Non-equalities: -6≠-5, 6≠7;
//  | | | | | | | |		Equality rules used: [F]
//  | | | | | | | |		(is ?)
//  | | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, 7:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:d, -8:¬c}	Equalities: 4=5=6;	Non-equalities: -8≠-7, -6≠-5, 6≠7;
//  | | | | | | | |		Equality rules used: [F]
//  | | | | | | | |		(is ?)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:b, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:d}	Equalities: 4=5;	Non-equalities: -6≠-5;
//  | | | | | | |		(is ?)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, 6:d, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:b}	Equalities: 4=5;	Non-equalities: -6≠-5;
//  | | | | | | |		(is ?)
//  | | | | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, 4:b, 5:¬c, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:c, -5:(b≡d), -6:(d≡¬c), -7:b, -8:d}	Equalities: 4=5;	Non-equalities: -8≠-7, -6≠-5;
//  | | | | | | |		(is ?)
//  | | | |--: Labels: {1:¬((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), 2:(b≡¬c), 3:a, -1:((b≡¬c)→(a→((b≡d)≡(d≡¬c)))), -2:(a→((b≡d)≡(d≡¬c))), -3:((b≡d)≡(d≡¬c)), -4:b, -5:¬c}	Equalities: -5=-4;	Non-equalities:;
//  | | | |		(is ?)
// a = 2
// b = 1

}

int main() {
  TestApplyRule();
  TestSolveTermination();
  TestCompare();
}