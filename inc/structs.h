#include "commons.h"

enum Operator {
    op_not, op_impl, op_equiv
};

struct Formula {
    // A formula is a variable, XOR
    bool is_var;
    int var;
    // An operator with one or two subformulas.
    Operator op;
    vector<Formula> subformulas;
};

struct Set {
    vector<Formula> formulas;
};

struct ProofNode {
    Set root;
    vector<ProofNode> subnodes;
};