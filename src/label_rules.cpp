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

bool LERNot(LabelNode &n) {
  for (int u = n.root.min_label + 1; u < n.root.max_label; u++) {
    if (u == 0 || !n.root.GetFormula(u).IsOp(op_not))
      continue;
    for (int y = u + 1; y < n.root.max_label; y++) {
      if (y == 0 || !n.root.GetFormula(y).IsOp(op_not))
        continue;
      if (n.root.IsEqual(u, y) || n.root.GetFormula(u) == n.root.GetFormula(y))
        continue;
      for (int w = n.root.min_label + 1; w < n.root.max_label; w++) {
        if (w == 0 ||
            !(n.root.GetFormula(w) == n.root.GetFormula(u).Subformula()))
          continue;
        for (int v = n.root.min_label + 1; v < n.root.max_label; v++) {
          if (v == 0 ||
              !(n.root.GetFormula(v) == n.root.GetFormula(y).Subformula()))
            continue;
          if (!n.root.IsEqual(w, v))
            continue;
          n.root.MakeEqual(u, y);
          return true;
        }
      }
    }
  }
  return false;
}

bool LERImpl(LabelNode &n) {
  int start = n.root.min_label + 1;
  int maxi = n.root.max_label;
  for (int x = start; x < maxi; x++) {
    if (x == 0 || !n.root.GetFormula(x).IsOp(op_impl))
      continue;
    for (int z = x + 1; z < maxi; z++) {
      if (z == 0 || !n.root.GetFormula(z).IsOp(op_impl))
        continue;
      if (n.root.IsEqual(x, z))
        continue;
      for (int w = start; w < maxi; w++) {
        if (w == 0 ||
            !(n.root.GetFormula(w) == n.root.GetFormula(x).Subformula(0)))
          continue;
        for (int v = start; v < maxi; v++) {
          if (v == 0 ||
              !(n.root.GetFormula(v) == n.root.GetFormula(z).Subformula(0)))
            continue;
          if (!n.root.IsEqual(w, v))
            continue;
          for (int u = start; u < maxi; u++) {
            if (u == 0 ||
                !(n.root.GetFormula(u) == n.root.GetFormula(x).Subformula(1)))
              continue;
            for (int y = start; y < maxi; y++) {
              if (y == 0 ||
                  !(n.root.GetFormula(y) == n.root.GetFormula(z).Subformula(1)))
                continue;
              if (!n.root.IsEqual(u, y))
                continue;
              n.root.MakeEqual(x, z);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool LEREq(LabelNode &n) {
  int start = n.root.min_label + 1;
  int maxi = n.root.max_label;
  for (int x = start; x < maxi; x++) {
    if (x == 0 || !n.root.GetFormula(x).IsOp(op_id))
      continue;
    for (int z = x + 1; z < maxi; z++) {
      if (z == 0 || !n.root.GetFormula(z).IsOp(op_id))
        continue;
      if (n.root.IsEqual(x, z))
        continue;
      for (int w = start; w < maxi; w++) {
        if (w == 0 ||
            !(n.root.GetFormula(w) == n.root.GetFormula(x).Subformula(0)))
          continue;
        for (int v = start; v < maxi; v++) {
          if (v == 0 ||
              !(n.root.GetFormula(v) == n.root.GetFormula(z).Subformula(0)))
            continue;
          if (!n.root.IsEqual(w, v))
            continue;
          for (int u = start; u < maxi; u++) {
            if (u == 0 ||
                !(n.root.GetFormula(u) == n.root.GetFormula(x).Subformula(1)))
              continue;
            for (int y = start; y < maxi; y++) {
              if (y == 0 ||
                  !(n.root.GetFormula(y) == n.root.GetFormula(z).Subformula(1)))
                continue;
              if (!n.root.IsEqual(u, y))
                continue;
              n.root.MakeEqual(x, z);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool LERF(LabelNode &n) {
  int start = n.root.min_label + 1;
  int maxi = n.root.max_label;
  for (int w = start; w < maxi; w++) {
    if (w == 0)
      continue;
    for (int v = w + 1; v < maxi; v++) {
      if (v == 0)
        continue;
      if (n.root.IsEqual(w, v))
        continue;
      if (n.root.GetFormula(v) == n.root.GetFormula(w)) {
        n.root.MakeEqual(v, w);
        return true;
      }
    }
  }
  return false;
}

string GelLabelEqRuleString(LabelEqRule rule) {
  if (rule == LERNot)
    return "≡¬";
  if (rule == LERImpl)
    return "≡→";
  if (rule == LEREq)
    return "≡≡";
  if (rule == LERF)
    return "F";

  assert(0);
}