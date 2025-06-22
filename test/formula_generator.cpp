#include "commons.h"
#include "generator.h"
#include "label_solver.h"
#include "preprocessing.h"

// Args:
// 1: number of formulas
// 2: size of formulas
// 3: number of variables of formulas
// 4: (optional) mode
//    - 0 = any (default)
//    - 1 = closed only

enum Mode { any = 0, closed_only = 1 };

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cout << "Args:\n"
            " 1: number of formulas \n"
            " 2: size of formulas \n"
            " 3: number of variables of formulas \n"
            " 4: (optional) mode \n"
            "    - 0 = any (default)\n"
            "    - 1 = closed only\n";
    return 0;
  }

  srand(time(0));

  char *p;

  int count = strtol(argv[1], &p, 10);

  int size = strtol(argv[2], &p, 10);

  int vars = strtol(argv[3], &p, 10);
  assert(vars <= 10);

  Mode mode = Mode::any;
  if (argc >= 4) {
    int arg4 = strtol(argv[4], &p, 10);
    assert(arg4 == 0 || arg4 == 1);
    mode = static_cast<Mode>(arg4);
  }

  for (int i = 0; i < count; i++) {
    auto f = GetRandomFormula(size, vars);

    if (mode == Mode::closed_only) {
      auto node = DoSolveLabel({f});
      if (!(node.is_closed.has_value() && node.is_closed.value())) {
        i--;
        continue;
      }
    }

    if (GetVariableCount(f) != vars) {
      i--;
      continue;
    }

    cout << f << endl;
    assert(size == GetSize(f));
  }
}