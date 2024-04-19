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

vector<LabelNode> LRNotMinus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_not)) {
    LabelNode to_return = n;
    to_return.root.CreateLabel(true, n.root.GetFormula(label).Subformula());
    return {to_return};
  }
  return {};
}

vector<LabelNode> LRImplPlus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_impl)) {
    const Formula &f = n.root.GetFormula(label);
    vector<LabelNode> to_return = {n, n, n};

    to_return[0].root.CreateLabel(false, f.Subformula(0));
    to_return[0].root.CreateLabel(false, f.Subformula(1));

    to_return[1].root.CreateLabel(false, f.Subformula(0));
    to_return[1].root.CreateLabel(true, f.Subformula(1));

    to_return[2].root.CreateLabel(true, f.Subformula(0));
    to_return[2].root.CreateLabel(true, f.Subformula(1));

    return to_return;
  }
  return {};
}

vector<LabelNode> LRImplMinus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_impl)) {
    const Formula &f = n.root.GetFormula(label);
    LabelNode to_return = n;
    to_return.root.CreateLabel(true, f.Subformula(0));
    to_return.root.CreateLabel(false, f.Subformula(1));
    return {to_return};
  }
  return {};
}

vector<LabelNode> LRIdPlus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_id)) {
    const Formula &f = n.root.GetFormula(label);
    vector<LabelNode> to_return = {n, n};

    int a = to_return[0].root.CreateLabel(true, f.Subformula(0));
    int b = to_return[0].root.CreateLabel(true, f.Subformula(1));
    to_return[0].root.MakeEqual(a, b);

    a = to_return[1].root.CreateLabel(false, f.Subformula(0));
    b = to_return[1].root.CreateLabel(false, f.Subformula(1));
    to_return[1].root.MakeEqual(a, b);
    return to_return;
  }
  return {};
}

vector<LabelNode> LRIdMinus(const LabelNode &n, int label) {
  if (n.root.GetFormula(label).IsOp(op_id)) {
    const Formula &f = n.root.GetFormula(label);
    vector<LabelNode> to_return = {n, n, n, n};

    int a = to_return[0].root.CreateLabel(true, f.Subformula(0));
    int b = to_return[0].root.CreateLabel(true, f.Subformula(1));
    to_return[0].root.MakeNotEqual(a, b);

    to_return[1].root.CreateLabel(true, f.Subformula(0));
    to_return[1].root.CreateLabel(false, f.Subformula(1));

    to_return[2].root.CreateLabel(false, f.Subformula(0));
    to_return[2].root.CreateLabel(true, f.Subformula(1));

    a = to_return[3].root.CreateLabel(false, f.Subformula(0));
    b = to_return[3].root.CreateLabel(false, f.Subformula(1));
    to_return[3].root.MakeNotEqual(a, b);

    return to_return;
  }
  return {};
}

string GetLabelRuleString(LabelRule rule) {
  if (rule == LRNotPlus)
    return "¬+";
  if (rule == LRNotMinus)
    return "¬-";
  if (rule == LRImplPlus)
    return "→+";
  if (rule == LRImplMinus)
    return "→-";
  if (rule == LRIdPlus)
    return "≡+";
  if (rule == LRIdMinus)
    return "≡-";
  assert(0);
}