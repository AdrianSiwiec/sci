#include "commons.h"
#include "rules.h"
#include "structs.h"

Formula f_not(op_not, {{op_not, {3}}});

void testRNot() {
  assert(RNot(Formula(5)).empty());
  assert(RNot(Formula(op_not, {5})).empty());

  assert(RNot(f_not) == vector<Set>{Set({Formula(3)})});
}

void testRImpl() {
  assert(RImpl({f_not}).empty());
}

int main() {
  testRNot();
  testRImpl();
}