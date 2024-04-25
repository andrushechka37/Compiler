
#include "tree.h"
#include <cstdio>
#include <cstring>
#include "read_to_tree.h"
#include "compiler.h"


#define two_arg_switch(num)                                 \
    case num:                                               \
        IR_array.data[IR_array.size].op_number = num;       \
        IR_array.data[IR_array.size].type = operator_class; \
        IR_array.size++;                                    \
        break;

                              


void print_struct(diff_tree_element * element, IR_elements IR_array) {

    if (element == NULL) {
        return;
    }

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

        IR_array.data[IR_array.size].op_number = OP_SQRT;  
        IR_array.data[IR_array.size].type = operator_class;
        IR_array.size++;   

    } else {
        
        if (ELEM_TYPE == value_class) {

            IR_array.data[IR_array.size].number = (int)ELEM_DOUBLE;
            IR_array.data[IR_array.size].type = value_class;
            IR_array.size++;  

        } else if (ELEM_TYPE == variable_class) {

                IR_array.data[IR_array.size].number = (int)ELEM_DOUBLE;       
                IR_array.data[IR_array.size].type = variable_class; 
                IR_array.size++;   

        } else if (ELEM_OP_NUM == OP_EQUAL) {

            if (!IS_ELEM(element->parent->parent, syntax_class, OP_IF) & !IS_ELEM(element->parent->parent, syntax_class, OP_WHILE)) {

                IR_array.data[IR_array.size].op_number = OP_MOVE;       
                IR_array.data[IR_array.size].type = syntax_class; 
                IR_array.size++;                                    
            
            } else {
                IR_array.data[IR_array.size].op_number = OP_EQUAL;       
                IR_array.data[IR_array.size].type = syntax_class; 
                IR_array.size++; 
            }
        } else if (ELEM_TYPE == function_class) {

            // if (element->right == NULL) {
            //     fprintf(pfile, "call :%lg\n", element->value.number);
            // } else {
            //     funcs[(int)element->value.number] = element->right;
            // }

        } else {

            if (ELEM_OP_NUM == OP_END) {

                print_struct(element->left, IR_array);
                print_struct(element->right, IR_array);

            } else if (ELEM_OP_NUM == OP_EQUAL) {

                print_struct(element->right, IR_array);
                print_struct(element->left, IR_array);
                
            } else if (ELEM_OP_NUM == OP_WHILE) {

                IR_array.data[IR_array.size].op_number = OP_WHILE;  
                IR_array.data[IR_array.size].type = syntax_class;
                IR_array.size++;   
                
            } else if (ELEM_OP_NUM == OP_IF ){

                IR_array.data[IR_array.size].op_number = OP_IF;  
                IR_array.data[IR_array.size].type = syntax_class;
                IR_array.size++;   

                
            } else if (ELEM_OP_NUM == OP_PRINT) {

                IR_array.data[IR_array.size].op_number = OP_PRINT;  
                IR_array.data[IR_array.size].type = syntax_class;
                IR_array.size++;   


            } else if (ELEM_OP_NUM == OP_RET) {

                IR_array.data[IR_array.size].op_number = OP_RET;  
                IR_array.data[IR_array.size].type = syntax_class;
                IR_array.size++;   

            } else {
                printf("unknown arg in print single command\n");
            }
        }
    }
}

void dump_IR(IR_elements IR_array[]) {
    FILE * pfile = fopen("IR_log.txt", "w");
    for (int i = 0; i < IR_array->size; i++) {
        fprintf(pfile, "%d | %d | %d | %d\n", IR_array->data[i].number, IR_array->data[i].op_number, IR_array->data[i].type);
    }
    fclose(pfile);
}



int main() {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    // IR_elements IR_array = {};
    // print_struct(tree, IR_array);
    // dump_IR(&IR_array);

    

    tree_visualize(tree);
    print_asm_code(tree);
    return 0;
}