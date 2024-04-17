#include "label_rules.h"
#include "commons.h"

vector<LabelNode> LRNotPlus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_not)) {
    LabelNode to_return = n;
    to_return.root.CreateLabel(false, n.root.GetFormula(label).Subformula());
    return {to_return};
  }
  return {};
}