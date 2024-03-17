#include "commons.h"
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
         Formula(op_equiv, {Formula(0), Formula(0)}));

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
  assert(Formula(op_equiv, {Formula(0), Formula(0)}) >
         Formula(op_impl, {Formula(0), Formula(0)}));
}

int main() {
  FormulaTest();
  FormulaOperatorTest();
}