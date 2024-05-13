#pragma once
#include "tree.h"

const int IR_element_len = 100;

struct IR_argument {
    types_of_node type;
    int op_number;
};

struct IR_element {
    int op_number;
    types_of_node type;
    IR_argument  first_arg;
    IR_argument second_arg;
    int countity_of_arg = 0;
};

struct IR_elements {
    IR_element data[IR_element_len];
    int size;
};