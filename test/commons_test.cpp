#include "commons.h"

void TestStringSplit() {
  assert(SplitString("a vdf r  l", " ") ==
         vector<string>({"a", "vdf", "r", "l"}));
  assert(SplitString("asdf", "|") == vector<string>({"asdf"}));
}
int main() { TestStringSplit(); }