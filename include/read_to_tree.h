#pragma once
#include "tree.h"
#include <cstdio>

int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent);

const int LABELS_QUANTITY = 10;
inline variables_info variables_table;
inline int labels_global[LABELS_QUANTITY] = {};

void print_asm_code(diff_tree_element * element);
diff_tree_element * read_tree();
int get_free_label(int labels[]);


#define IS_ELEM(element, type_of_node, value_of_node) (element->type == type_of_node && \
                                                       element->value.operator_info.op_number == value_of_node)
                                                       
#define ARG_CASES(arg, text)        \
    case arg:                       \
        fprintf(pfile, text);       \
        break;

#define END_OF_SWITCH                                      \
    default:                                               \
        printf("unknown arg - %d, %s, %s, %d!!!!!!!\n", element->value.operator_info.op_number, __FILE__, __func__, __LINE__);  \
        break;


#define IF_CASES(arg, text)        \
    case arg:                      \
        fprintf(pfile, text, end); \
        break;
