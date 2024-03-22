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
}

int main() {
  TestProcessVarChar();
  TestPreprocessing();
}
