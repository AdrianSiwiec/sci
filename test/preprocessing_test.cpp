#include "commons.h"
#include "preprocessing.h"

void TestProcessVarChar() {
  string s = "a-ag4-a-0";
  int pos = 0;
  ProcessVarChar(s, pos);
  assert(s == "0-ag4-a-0");
  assert(pos == 1);

  pos++;
  ProcessVarChar(s, pos);
  assert(s == "0-1-a-0");
  assert(pos == 3);

  pos++;
  ProcessVarChar(s, pos);
  assert(s == "0-1-0-0");
  assert(pos == 5);

  pos++;
  ProcessVarChar(s, pos);
  assert(s == "0-1-0-2");
  assert(pos == 7);

  ClearVars();
}

void TestPreprocessing() {
  assert(PreprocessInput("  a - ag2 43-a- 0  ") == "(0-1-0-2)");
  ClearVars();
}

void TestInput(string s, optional<Formula> f) {
  auto result = ParseInput(s);
  if (f != result) {
    cout << "Testing: " << s << endl;
    if (result.has_value())
      cout << "Got: " << result.value() << endl;
    else
      cout << "Got Error!" << endl;

    if (f.has_value())
      cout << "Expected: " << f.value() << endl;
    else
      cout << "Expected Error" << endl;
  }
  assert(result == f);
  ClearVars();
}

void TestInputsFromPaper() {
  TestInput("f=f", Formula(op_equiv, {{0}, {0}}));
  TestInput("(phi=psi)->(phi=psi)",
            Formula(op_impl, {{op_equiv, {{0}, {1}}}, {op_equiv, {{0}, {1}}}}));
  TestInput("(phi=psi)->(-phi=-psi)",
            Formula(op_impl, {{op_equiv, {{0}, {1}}},
                              {op_equiv, {{op_not, {{0}}}, {op_not, {{1}}}}}}));
  TestInput(
      "(phi=psi)->((chi=theta)->((phi->chi)=(psi->theta)))",
      Formula(op_impl, {{op_equiv, {{0}, {1}}},
                        {op_impl,
                         {{op_equiv, {{2}, {3}}},
                          {op_equiv,
                           {{op_impl, {{0}, {2}}}, {op_impl, {{1}, {3}}}}}}}}));
  TestInput("--f", Formula(op_not, {{op_not, {{0}}}}));
}

int main() {
  TestProcessVarChar();
  TestPreprocessing();
  TestInputsFromPaper();
}
