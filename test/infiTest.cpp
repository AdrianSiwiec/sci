#include "commons.h"
#include "generator.h"
#include "label_solver.h"

void Test(int i, const Formula &f) {
  cout << "Testing " << i << ": " << f;
  cout << "\t\t" << (TestFormula(f) ? "(closed)" : "(open)");
  cout << endl;
}

int main() {
  for (int i = 0; i < 10000; i++) {
    Formula f = GetRandomFormula(5, 2);
    Test(i, f);
    Test(i, Formula(op_not, {f}));
  }
}