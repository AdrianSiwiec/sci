#include "commons.h"
#include "preprocessing.h"
#include "structs.h"

void FormulaTest() {
  Formula f(4);
  assert(f.IsVar(4));
  assert(!f.IsVar(5));
  assert(!f.IsOp(op_not));
}

void FormulaOperatorTest() {
  assert(Formula(1) == Formula(1));
  assert(Formula(2) != Formula(0));
  assert(Formula(op_not, {Formula(0)}) !=
         Formula(op_impl, {Formula(0), Formula(0)}));
  assert(Formula(op_impl, {Formula(0), Formula(0)}) ==
         Formula(op_impl, {Formula(0), Formula(0)}));
  assert(Formula(op_impl, {Formula(0), Formula(0)}) !=
         Formula(op_impl, {Formula(0), Formula(1)}));
  assert(Formula(op_impl, {Formula(0), Formula(0)}) !=
         Formula(op_id, {Formula(0), Formula(0)}));

  assert(Formula(3) < Formula(4));
  assert(Formula(4) > Formula(3));
  assert(Formula(4) < Formula(op_not, {Formula(0)}));
  assert(Formula(op_not, {Formula(0)}) <
         Formula(op_impl, {Formula(0), Formula(0)}));
  assert(!(Formula(op_impl, {Formula(0), Formula(0)}) <
           Formula(op_impl, {Formula(0), Formula(0)})));
  assert(!(Formula(op_impl, {Formula(0), Formula(0)}) >
           Formula(op_impl, {Formula(0), Formula(0)})));
  assert(Formula(op_impl, {Formula(0), Formula(1)}) >
         Formula(op_impl, {Formula(0), Formula(0)}));
  assert(Formula(op_impl, {Formula(1), Formula(0)}) >
         Formula(op_impl, {Formula(0), Formula(9)}));
  assert(Formula(op_id, {Formula(0), Formula(0)}) >
         Formula(op_impl, {Formula(0), Formula(0)}));
}

void SetTest() {
  Set s({Formula(3), Formula(2), Formula(1)});
  assert(s == Set({Formula(1), Formula(2), Formula(3)}));

  Set s1(s.Formulas());
  s1.RemoveFormula(2);
  assert(s1.Formulas().size() == 2);
  assert(s1 == Set({Formula(1), Formula(2)}));
  s1.ReplaceFormula(0, Formula(5));
  assert(s1 == Set({Formula(2), Formula(5)}));

  s.ReplaceFormula(2, Formula(5));
  s.ReplaceFormula(0, Formula(5));
  assert(s == s1);

  s1.AddFormula(Formula(5));
  assert(s == s1);

  s1.ReplaceFormula(0, Formula(5));
  assert(s1 == Set({Formula(5)}));
  s1.RemoveFormula(0);
  assert(s1 == Set(vector<Formula>()));

  s1.AddFormula(Formula(7));
  s.ReplaceFormula(0, Formula(9));
  s.ReplaceFormula(0, Formula(9));
  assert(s != s1);
  s.ReplaceFormula(0, Formula(7));
  assert(s == s1);

  s = Set("0,1,2");
  assert(s == Set({Formula(1), Formula(2), Formula(0)}));
  ClearVars();
}

int main() {
  FormulaTest();
  FormulaOperatorTest();
  SetTest();
}