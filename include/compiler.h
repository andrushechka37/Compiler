#pragma once
#include "tree.h"

const int IR_element_len = 100;

struct IR_argument {
    types_of_node type;
    int op_number;
};

struct IR_element {
    int op_number;
    int junk_for_junc;
    types_of_node type;
    IR_argument  first_arg;
    int countity_of_arg = 0;
    int label;
};

struct IR_elements {
    IR_element data[IR_element_len];
    int size;
};

void make_IR_array(diff_tree_element * element, IR_elements * IR_array);