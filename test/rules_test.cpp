#include "commons.h"
#include "rules.h"
#include "structs.h"

void testRNot() {
  assert(RNot(Set({Formula(5)})).empty());
  assert(RNot(Set({Formula(op_not, {Formula(5)})})).empty());
}

int main() { testRNot(); }