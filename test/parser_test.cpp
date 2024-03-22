#include "commons.h"
#include "structs.h"

void TestParseVar() {
  int pos = 0;
  assert(ParseVar("asd", pos) == nullopt);
  assert(pos == 0);

  assert(ParseVar("23", pos) == 23);
  assert(pos == 2);

  assert(ParseVar("aa4b", pos) == 4);
  assert(pos == 3);
}

void TestParseOp() {
  int pos = 0;
  assert(ParseOp("-", pos) == nullopt);

  assert(ParseOp("=", pos) == op_equiv);
  assert(pos == 1);

  assert(ParseOp("a->b", pos) == op_impl);
  assert(pos == 3);
}

void TestParser(string s, optional<Formula> f) {
  int pos = 0;
  assert(Formula::Parse(s, pos) == f);
  if (f.has_value()) {
    assert(pos == s.size());
  }
}

void TestParser() {
  TestParser("d", nullopt);
  TestParser("1", nullopt);

  TestParser("(1)", Formula(1));
  TestParser("(-(5))", Formula(op_not, {{5}}));
  TestParser("((5)->(3))", Formula(op_impl, {{5}, {3}}));
  TestParser("((8)=(90))", Formula(op_equiv, {{8}, {90}}));
  TestParser("((-(7))=((3)=((4)->(1))))",
             Formula(op_equiv, {{op_not, {{7}}},
                                {op_equiv, {{3}, {op_impl, {{4}, {1}}}}}}));
}

int main() {
  TestParseVar();
  TestParseOp();
  TestParser();
}