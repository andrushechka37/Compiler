
#include "tree.h"
#include <cstdio>
#include <cstring>
#include "read_to_tree.h"
#include "compiler.h"

// TODO: print single better
// TODO: better dump with names


#define two_arg_switch(num)                                    \
    case num:                                                  \
        IR_array->data[IR_array->size].op_number = num;        \
        IR_array->data[IR_array->size].type = operator_class;  \
        IR_array->size++;                                      \
        break;


#define IS_NULL(element)      \
    if (element == NULL) {    \
        return;               \
    }                         

#define SET_IR_ELEM(IR_type, IR_number)                             \
    IR_array->data[IR_array->size].op_number = IR_number;           \
    IR_array->data[IR_array->size].type = IR_type;                  \
    IR_array->size++;   

void print_struct(diff_tree_element * element, IR_elements * IR_array) {

    IS_NULL(element);

    if (ELEM_OP_ARG == 2) {

        print_struct(element->left, IR_array);
        print_struct(element->right, IR_array);

        switch (ELEM_OP_NUM) {

            two_arg_switch(OP_ADD);
            two_arg_switch(OP_SUB);
            two_arg_switch(OP_MUL);
            two_arg_switch(OP_DIV);

            END_OF_SWITCH;
        }

    } else if (ELEM_OP_ARG == 1) {

        SET_IR_ELEM(operator_class, OP_SQRT);

    } else {
        
        if (ELEM_TYPE == value_class) {

            SET_IR_ELEM(value_class, (int)ELEM_DOUBLE);

        } else if (ELEM_TYPE == variable_class) {

            SET_IR_ELEM(variable_class, (int)ELEM_DOUBLE);

        } else if (ELEM_OP_NUM == OP_EQUAL) {

            print_struct(element->right, IR_array);
            print_struct(element->left, IR_array);

            if (!IS_ELEM(element->parent, syntax_class, OP_IF) & !IS_ELEM(element->parent, syntax_class, OP_WHILE)) {

                 SET_IR_ELEM(syntax_class, OP_MOVE);                            
            
            } else {

                SET_IR_ELEM(syntax_class, OP_EQUAL);
            }
        } else if (ELEM_TYPE == function_class) {

            // if (element->right == NULL) {
            //     fprintf(pfile, "call :%lg\n", element->value.number);
            // } else {
            //     funcs[(int)element->value.number] = element->right;
            // }

        } else {

            switch (ELEM_OP_NUM) {

                case OP_END:
                    print_struct(element->left, IR_array);
                    print_struct(element->right, IR_array);
                    break;

                case OP_WHILE:
                    SET_IR_ELEM(syntax_class, OP_WHILE);
                    break;

                case OP_IF:
                    SET_IR_ELEM(syntax_class, OP_IF);
                    break;
                
                case OP_PRINT:
                    SET_IR_ELEM(syntax_class, OP_PRINT);
                    break;
                
                case OP_RET:
                    SET_IR_ELEM(syntax_class, OP_RET);
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
        fprintf(pfile, "%d | %d | %d | %d\n", IR_array->data[i].op_number, IR_array->data[i].op_number, IR_array->data[i].type);
    }
    fclose(pfile);
}



int main() {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    IR_elements IR_array = {};
    print_struct(tree, &IR_array);
    dump_IR(&IR_array);

    

    tree_visualize(tree);
    print_asm_code(tree);
    return 0;
}