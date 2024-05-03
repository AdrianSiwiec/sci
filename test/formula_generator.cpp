#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"

int main() {
  long long a, b;
  cin >> a >> b;
  srand(a * b + b);
  int count;
  cin >> count;
  cout << count << endl;
  for (int i = 0; i < count; i++) {
    int variables = 2;
    if ((rand() % 3) == 0)
      variables = 3;
    if ((rand() % 10) == 0)
      variables = 4;
    if ((rand() % 10) == 0)
      variables = 7;
    cout << GetRandomFormula((rand() % 6) + 4, variables) << endl;
  }
}