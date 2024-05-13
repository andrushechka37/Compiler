#include "../include/tree.h"
#include <cstdio>
#include <cstring>
#include "../include/read_to_tree.h"
#include "../include/compiler.h"

// TODO: print single better
// TODO: make IR with list

// TODO: IR dump more complex
// TODO: better dump with names
// TODO: comments when generate

// NOTE: пройтись по постройке аяра, почекать че пишет, написать асм для того что есть



void set_IR_e(diff_tree_element * element, IR_elements * IR_array);

#define ELEM_ARGUMENT(argument, info) IR_array->data[IR_array->size - 1].argument.info

// I have a better idea to fix countity of argumnets in table, put this into set_IR_element
// there will be a check on countity of args, 
void set_arguments_for_IR_element(diff_tree_element * first_arg, 
                                  diff_tree_element * second_arg, 
                                  IR_elements       * IR_array) {

    if (first_arg != NULL) {
        ELEM_ARGUMENT(first_arg, type) = first_arg->type;
        ELEM_ARGUMENT(first_arg, op_number) = first_arg->value.number;
    }

    if (second_arg != NULL) {
        ELEM_ARGUMENT(second_arg, type) = second_arg->type;
        ELEM_ARGUMENT(second_arg, op_number) = second_arg->value.number;
    }
}



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

        set_arguments_for_IR_element(element->left, element->right, IR_array);

        IR_array->data[IR_array->size - 1].countity_of_arg = 2; // -----------TOFIX: !!!!!!!!!!!!!! it is costul



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
            fprintf(pfile, "%s \n", get_op_symbol(IR_array->data[i].op_number), IR_array->data[i].type);
        } else if (IR_array->data[i].type == variable_class) {
            fprintf(pfile, " %d variable %s\n", IR_array->data[i].op_number, variables_table.table[IR_array->data[i].op_number].name);
        } else if (IR_array->data[i].type == value_class) {
            fprintf(pfile, " %d value \n", IR_array->data[i].op_number);
        }
    }
    fclose(pfile);
}


void print_x86_command(IR_element * element, FILE * pfile) {

    if (element->countity_of_arg == 2 && element->op_number != OP_EQUAL) {
        fprintf(pfile, "\n; start of %s\n", get_op_x86_name(element->op_number));
        fprintf(pfile, "pop r12\n");
        fprintf(pfile, "pop r13\n");
        fprintf(pfile, "%s r13, r12\n", get_op_x86_name(element->op_number));
        fprintf(pfile, "push r13\n");
        fprintf(pfile, "; end of %s\n\n", get_op_x86_name(element->op_number));

    } else if (element->countity_of_arg == 1) {

        fprintf(pfile, "%s ", get_op_x86_name(element->op_number));

        if (element->first_arg.type == value_class) {
            fprintf(pfile, "%d\n", element->first_arg.op_number);
        } else {
            fprintf(pfile, "r%cx\n", element->first_arg.op_number + 'a');
        }
    } else {
        fprintf(pfile, "%s\n", get_op_x86_name(element->op_number));
    }
}

void print_x86_asm(IR_elements * IR_array) {

    FILE * pfile = fopen("x86.s", "w");

    fprintf(pfile, "section .text\n");
    fprintf(pfile, "main:\n");


    for (int i = 0; i < IR_array->size; i++) {
        print_x86_command(&(IR_array->data[i]), pfile);
    }

    fclose(pfile);
}

int main() {

    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    IR_elements IR_array = {};
    
    set_IR_e(tree, &IR_array);
    dump_IR(&IR_array);

    tree_visualize(tree);
    print_asm_code(tree);
    print_x86_asm(&IR_array);
    return 0;
}




#define TWO_ARG(num)                                \
    case num:                                       \
    set_IR_element(IR_array, operator_class, num);  \
    break;                      



void set_IR_e(diff_tree_element * element, IR_elements * IR_array) {

    IS_NULL(element);

    if (ELEM_OP_ARG == 2) {

        if (ELEM_OP_NUM == OP_EQUAL) {

            set_IR_e(element->right, IR_array);
            set_IR_e(element->left, IR_array);
            return;
        } 

        set_IR_e(element->left, IR_array);
        set_IR_e(element->right, IR_array);


        switch (ELEM_OP_NUM) {

            TWO_ARG(OP_ADD);
            TWO_ARG(OP_SUB);
            TWO_ARG(OP_MUL);
            TWO_ARG(OP_DIV);

            END_OF_SWITCH;
        }
        IR_array->data[IR_array->size - 1].countity_of_arg = 2;

    } else {
        
        if (ELEM_TYPE == value_class) {

            set_IR_element(IR_array, syntax_class, OP_PUSH);
            
            ELEM_ARGUMENT(first_arg, type) = value_class;
            ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;
            IR_array->data[IR_array->size - 1].countity_of_arg = 1;

        } else if (ELEM_TYPE == variable_class) {

            if (IS_ELEM(element->parent, syntax_class, OP_EQUAL) & !IS_ELEM(element->parent->parent, syntax_class, OP_IF)
                                                             & !IS_ELEM(element->parent->parent, syntax_class, OP_WHILE)) {

                set_IR_element(IR_array, syntax_class, OP_POP);
            
                ELEM_ARGUMENT(first_arg, type) = variable_class;
                ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;
                IR_array->data[IR_array->size - 1].countity_of_arg = 1;
            } else {
                set_IR_element(IR_array, syntax_class, OP_PUSH);
            
                ELEM_ARGUMENT(first_arg, type) = variable_class;
                ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;
                IR_array->data[IR_array->size - 1].countity_of_arg = 1;
            }

        } else if (ELEM_TYPE == function_class) {

        } else {

            if (ELEM_OP_NUM == OP_END) {

                set_IR_e(element->left, IR_array);
                set_IR_e(element->right, IR_array);

            } else if (ELEM_OP_NUM == OP_WHILE) {

            } else if (ELEM_OP_NUM == OP_IF ){

            } else if (ELEM_OP_NUM == OP_PRINT) {
                
            } else if (ELEM_OP_NUM == OP_RET) {

                set_IR_element(IR_array, syntax_class, OP_RET);

            } else {
                printf("unknown arg in print single command\n");
            }
        }
    }
}