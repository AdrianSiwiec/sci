#include "commons.h"
#include "generator.h"
#include "label_solver.h"

int main() {
  for (int i = 0; i < 10000; i++) {
    Formula f = GetRandomFormula(5, 2);
    cout << "Testing "<<i<<": " << f;
    cout << "\t\t"<< (TestFormula(f) ? "(closed)" : "(open)");
    cout << endl;
  }
}