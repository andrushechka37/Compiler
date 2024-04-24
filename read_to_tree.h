#pragma once
#include "tree.h"
#include <cstdio>

bool check_symbol(char symbol, FILE * pfile);
int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent);
void set_type_value(diff_tree_element * element, double number, types_of_node type);

const int LABELS_QUANTITY = 10;
inline variables_info variables_table;
inline int labels_global[LABELS_QUANTITY] = {};

diff_tree_element * read_tree();