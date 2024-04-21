#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"

void Test(int i, const Formula &f) {
  bool print = i % 100 == 0;
  if (print)
    cout << "Testing " << i << ": " << f << flush;
  bool result = TestFormula(f);
  if (print) {
    cout << "\t\t" << (result ? "(closed)" : "(open)");
    cout << endl;
  }
}

int main() {
  srand(time(0));
  for (int i = 0; true; i++) {
    int variables = 2;
    if ((rand() % 3) == 0)
      variables = 3;
    if ((rand() % 10) == 0)
      variables = 4;
    if ((rand() % 10) == 0)
      variables = 7;
    // Formula f = GetRandomFormula((rand()%4)+4, variables);
    Formula f = GetRandomFormula((rand() % 4) + 3, variables);
    Test(i, f);
    Test(i, Formula(op_not, {f}));
    ClearVars();
  }
}