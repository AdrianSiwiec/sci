#include "commons.h"
#include "structs.h"

int main() {
  cout << "Hello There!" << endl;
  cout << "General Kenobi!" << endl;

  Formula f(5);
  cout<<f<<endl;

  Formula g(op_impl, {f, f});
  cout<<g<<endl;
  cout<<Formula(op_equiv, {g, Formula(op_not, {g})});
}