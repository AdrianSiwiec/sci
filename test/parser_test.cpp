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

  assert(ParseOp("=", pos) == op_id);
  assert(pos == 1);

  assert(ParseOp("a->b", pos) == op_impl);
  assert(pos == 3);

  pos = 1;
  assert(ParseOp("a&b", pos) == op_and);
  assert(pos == 2);
  
  pos = 1;
  assert(ParseOp("a|b", pos) == op_or);
  assert(pos == 2);

  pos = 1;
  assert(ParseOp("a<>b", pos) == op_equiv);
  assert(pos == 3);
}

void TestParser(string s, optional<Formula> f) {
  // if (f.has_value()) {
  //   cout << "Testing if: " << s << "\n  Parses as: " << f.value() << endl;
  //   int pos = 0;
  //   cout << "\tIt parses as: " << Formula::Parse(s, pos).value() << endl;
  // } else {
  //   cout << "Testing if: " << s << "\n Doesn't parse." << endl;
  // }
  int pos = 0;
  assert(Formula::Parse(s, pos) == f);
  if (f.has_value()) {
    assert(pos == s.size());
  } else {
    assert(pos == 0);
  }
}

void TestParserSimple() {
  TestParser("d", nullopt);
  TestParser("1", Formula(1));

  TestParser("(1)", Formula(1));
  TestParser("(-(5))", Formula(op_not, {{5}}));
  TestParser("((5)->(3))", Formula(op_impl, {{5}, {3}}));
  TestParser("((8)=(90))", Formula(op_id, {{8}, {90}}));
  TestParser("((-(7))=((3)=((4)->(1))))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("(1&2)", Formula(op_and, {{1}, {2}}));
  TestParser("(1|2)", Formula(op_or, {{1}, {2}}));
  TestParser("(1<>2)", Formula(op_equiv, {{1}, {2}}));
  TestParser("((1<>2)|(3&4))", Formula(op_or, {Formula(op_equiv, {{1}, {2}}), Formula(op_and, {{3},{4}})}));
}
void TestParserNotAllParen() {
  TestParser("-3", Formula(op_not, {{3}}));
  TestParser("((-7)=((3)=((4)->(1))))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("((-7)=(3=((4)->(1))))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("((-7)=(3=(4->(1))))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("((-7)=(3=(4->1)))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("(-7=(3=(4->1)))",
             Formula(op_id, {{op_not, {{7}}},
                                {op_id, {{3}, {op_impl, {{4}, {1}}}}}}));
  TestParser("((1=0)->(1->0))",
             Formula(op_impl, {{op_id, {{1}, {0}}}, {op_impl, {{1}, {0}}}}));
  TestParser("((((1=0)->(1->0))))",
             Formula(op_impl, {{op_id, {{1}, {0}}}, {op_impl, {{1}, {0}}}}));
}

int main() {
  TestParseVar();
  TestParseOp();
  TestParserSimple();
  TestParserNotAllParen();
}