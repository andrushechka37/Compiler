#include "tree.h"
#include <cstdio>
#include <cstring>
#include "read_to_tree.h"
#include "compiler.h"

// TODO: print single better
// TODO: better dump with names
// TODO: make func to set IR token 
// TODO: IR dump more complex
// TODO: make IR with list
// TODO: comments when generate

void set_IR_element(IR_elements * IR_array, types_of_node type, int num) {

    IR_array->data[IR_array->size].op_number = num;        
    IR_array->data[IR_array->size].type = type;  
    IR_array->size++;    
    return;
}

#define two_arg_switch(num)                                    \
    case num:                                                  \
        set_IR(element->left, IR_array);                       \
        set_IR(element->right, IR_array);                      \
                                                               \
        set_IR_element(IR_array, operator_class, num);         \
        break;


#define IS_NULL(element)      \
    if (element == NULL) {    \
        return;               \
    }                         

// #define SET_IR_ELEM(IR_type, IR_number)                             \
//     IR_array->data[IR_array->size].op_number = IR_number;           \
//     IR_array->data[IR_array->size].type = IR_type;                  \
//     IR_array->size++;   



void set_IR(diff_tree_element * element, IR_elements * IR_array) {

    IS_NULL(element);

    if (ELEM_OP_ARG == 2) {

        switch (ELEM_OP_NUM) {

            two_arg_switch(OP_ADD);
            two_arg_switch(OP_SUB);
            two_arg_switch(OP_MUL);
            two_arg_switch(OP_DIV);

            case OP_EQUAL:
                set_IR(element->right, IR_array);
                set_IR(element->left,  IR_array);

                if (!IS_ELEM(element->parent, syntax_class, OP_IF) && !IS_ELEM(element->parent, syntax_class, OP_WHILE)) {
                    set_IR_element(IR_array, syntax_class, OP_MOVE);                           
                } else {
                    set_IR_element(IR_array, syntax_class, OP_EQUAL);
                }
                break;

            END_OF_SWITCH;
        }

    } else if (ELEM_OP_ARG == 1) {

        set_IR(element->right, IR_array);
        set_IR_element(IR_array, operator_class, OP_SQRT);

    } else {

        switch (ELEM_TYPE) {

            case value_class:
                set_IR_element(IR_array, value_class, (int)ELEM_DOUBLE);
                break;

            case variable_class:
                set_IR_element(IR_array, variable_class, (int)ELEM_DOUBLE);
                break;

            case function_class:
                // !!!!!!!!!!!!!!!!!!!!!
                break;
            
            default:

                switch (ELEM_OP_NUM) {

                case OP_END:
                    set_IR(element->left, IR_array);
                    set_IR(element->right, IR_array);
                    break;

                case OP_WHILE:
                    set_IR_element(IR_array, syntax_class, OP_WHILE);
                    break;

                case OP_IF:
                    set_IR_element(IR_array, syntax_class, OP_IF);
                    break;
                
                case OP_PRINT:
                    set_IR_element(IR_array, syntax_class, OP_PRINT);
                    break;
                
                case OP_RET:
                    set_IR_element(IR_array, syntax_class, OP_RET);
                    break;

                default:
                    printf("unknown arg in print single command\n");
            }
        }
    }
}

void dump_IR(IR_elements IR_array[]) {
    FILE * pfile = fopen("IR_log.txt", "w");
    for (int i = 0; i < IR_array->size; i++) {

        if (IR_array->data[i].type != value_class && IR_array->data[i].type != variable_class) {
            fprintf(pfile, "%s \n\n", get_op_symbol(IR_array->data[i].op_number), IR_array->data[i].type);
        } else if (IR_array->data[i].type == variable_class) {
            fprintf(pfile, " %d variable \n", IR_array->data[i].op_number);
        } else if (IR_array->data[i].type == value_class) {
            fprintf(pfile, " %d value \n", IR_array->data[i].op_number);
        }
    }
    fclose(pfile);
}



int main() {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    IR_elements IR_array = {};
    
    set_IR(tree, &IR_array);
    dump_IR(&IR_array);

    tree_visualize(tree);
    print_asm_code(tree);
    return 0;
}