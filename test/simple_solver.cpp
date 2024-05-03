#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"
#include "solver.h"

int main() {
  int count;
  cin >> count;
  string tmp;
  getline(cin, tmp);
  for (string line; getline(cin, line);) {
    ClearVars();
    auto formulas = DoParseFormulas(line, false);
    auto node = DoSolve({FNot(formulas[0])}, false);
    if (node.is_closed.value())
      cout << "proven" << endl;
    else
      cout << "unproven" << endl;
  }
}