#pragma once
#include "tree.h"


const int IR_element_len = 100;
struct IR_element {
    operations op_number;
    int number;
    types_of_node type;
};

struct IR_elements {
    IR_element data[IR_element_len];
    int size;
};