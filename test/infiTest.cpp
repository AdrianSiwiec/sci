#include "commons.h"
#include "preprocessing.h"
#include "generator.h"
#include "label_solver.h"

void Test(int i, const Formula &f) {
  cout << "Testing " << i << ": " << f<<flush;
  cout << "\t\t" << (TestFormula(f) ? "(closed)" : "(open)");
  cout << endl;
}

int main() {
  srand(time(0));
  for (int i = 0; i < 10000; i++) {
    Formula f = GetRandomFormula(5, 1);
    Test(i, f);
    Test(i, Formula(op_not, {f}));
    ClearVars();
  }
}