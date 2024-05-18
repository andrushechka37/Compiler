#include "../include/tree.h"
#include <cstdio>
#include <cstring>
#include "../include/read_to_tree.h"
#include "../include/compiler.h"


void set_IR_element(IR_elements * IR_array, types_of_node type, int num) {

    IR_array->data[IR_array->size].op_number = num;        
    IR_array->data[IR_array->size].type = type;  
    IR_array->size++;    
    return;
}

void dump_IR(IR_elements IR_array[]) {

    FILE * pfile = fopen("IR_log.txt", "w");
    for (int i = 0; i < IR_array->size; i++) {

        switch (IR_array->data[i].type) {

            case value_class:
                fprintf(pfile, " %d value type \n", IR_array->data[i].op_number);
                break;

            case operator_class:

                fprintf(pfile, "%s operator class", get_op_symbol(IR_array->data[i].op_number));
                if (IR_array->data[i].countity_of_arg == 1) {
                    if (IR_array->data[i].first_arg.type == value_class) {
                        fprintf(pfile, "arg is value %d\n", IR_array->data[i].first_arg.op_number);
                    } else {
                        fprintf(pfile, "arg is variable %d\n", IR_array->data[i].first_arg.op_number);
                    }
                } else {
                    fprintf(pfile, "\n");
                }

                break;

            case variable_class:
                fprintf(pfile, " %d variable %s\n", IR_array->data[i].op_number, variables_table.table[IR_array->data[i].op_number].name);
                break;

            case syntax_class:
                fprintf(pfile, "%s ", get_op_symbol(IR_array->data[i].op_number));

                if (IR_array->data[i].countity_of_arg == 1) {
                    if (IR_array->data[i].first_arg.type == value_class) {
                        fprintf(pfile, "arg is value %d\n", IR_array->data[i].first_arg.op_number);
                    } else {
                        fprintf(pfile, "arg is variable %d\n", IR_array->data[i].first_arg.op_number);
                    }
                } else {
                    fprintf(pfile, "no arguments\n");
                }
                break;

            case function_class:
                fprintf(pfile, "function aaaa\n");
                break;

            default:
                fprintf(pfile, "errrrrrieeeeeeee\n");
        }
    }
    fclose(pfile);
}

#define IF_CASES_LABEL(arg, text)                   \
    case arg:                                       \
        fprintf(pfile, text, element->label + 'a'); \
        break;


void print_x86_command(IR_element * element, FILE * pfile) {

    if (element->countity_of_arg == 2 && element->op_number != OP_EQUAL) { // for math operations

        fprintf(pfile, "\n; start of %s\n", get_op_x86_name(element->op_number)); // TODO: remove hardcode of r12 r13

        if (element->op_number == OP_MUL) { // TODO: rewrite 
            
            fprintf(pfile, "mov r15, rax\n");

            fprintf(pfile, "pop rax\n");
            fprintf(pfile, "pop r13\n");

            fprintf(pfile, "mul r13\n");
            fprintf(pfile, "push rax\n");

            fprintf(pfile, "mov rax, r15\n");

        } else {

            fprintf(pfile, "pop r12\n");
            fprintf(pfile, "pop r13\n");
            fprintf(pfile, "%s r13, r12\n", get_op_x86_name(element->op_number));
            fprintf(pfile, "push r13\n");
        }

        fprintf(pfile, "; end of %s\n\n", get_op_x86_name(element->op_number));


    } else if (element->countity_of_arg == 1 && !(element->op_number == OP_FUNC)) { // it is for push/pop registers or value

        fprintf(pfile, "%s ", get_op_x86_name(element->op_number));

        if (element->first_arg.type == value_class) {
            fprintf(pfile, "%d\n", element->first_arg.op_number);
        } else {
            fprintf(pfile, "r%cx ; %s\n", element->first_arg.op_number + 'a', get_variable_name(element->op_number));
        }

    } else {

        switch (element->op_number) {

            case OP_LABEL:
                fprintf(pfile, "%c:\n", element->label + 'a');
                break;

            case OP_JUMP:
                fprintf(pfile, "jmp %c\n", element->label + 'a');
                break;

            case OP_MORE:
            case OP_LESS:
            case OP_EQUAL:
            case OP_NEQUAL:

                fprintf(pfile, "\n; start of cmp\n"); // TODO: just strange thing
                fprintf(pfile, "pop r12\n");
                fprintf(pfile, "pop r13\n");
                fprintf(pfile, "cmp r13, r12\n");

                switch (element->op_number) {

                    IF_CASES_LABEL(OP_EQUAL, "jne %c\n");    
                    IF_CASES_LABEL(OP_MORE,  "jbe %c\n");    // TODO: fix intendention
                    IF_CASES_LABEL(OP_LESS,  "jae %c\n"); 
                    IF_CASES_LABEL(OP_NEQUAL, "je %c\n"); 

                    default:
                        printf("fggdfdgbdfbf");
                }

                fprintf(pfile, "; end of cmp\n\n"); // TODO: just strange thing
                break;

            case OP_FUNC:

                if (element->countity_of_arg == 1) {
                    fprintf(pfile, "\n\n; start of declaration of func\n");
                    fprintf(pfile, "jmp %c\n", element->label + 'a');
                    fprintf(pfile, "%s:\n", get_variable_name(element->junk_for_junc));
                } else {
                    fprintf(pfile, "call %s\n", get_variable_name(element->junk_for_junc));
                }
                break;
            
            case OP_PRINT:

                fprintf(pfile, "\n\n ;just print-------------\n");
                
                fprintf(pfile, "push rax\n");

                fprintf(pfile, "push r%cx\n", element->first_arg.op_number + 'a');
                fprintf(pfile, "pop rax\n");

                fprintf(pfile, "call print_integer\n");

                fprintf(pfile, "push rsi\n");               //
                fprintf(pfile, "mov rsi, neg_flag\n");      //
                fprintf(pfile, "mov byte [rsi], 10\n");     // printf("\n");
                fprintf(pfile, "call print_char\n");        //
                fprintf(pfile, "pop rsi\n");                //

                fprintf(pfile, "pop rax\n");

                fprintf(pfile, "\n\n ;just print-------------\n");

                break;



            default:
                fprintf(pfile, "%s\n", get_op_x86_name(element->op_number)); // TODO: finish work



        
        }
    }
}

void print_x86_asm(IR_elements * IR_array) {

    FILE * pfile = fopen("x86.s", "w");

    fprintf(pfile, "%%include \"andy_lib.s\"\n");
    fprintf(pfile, "global _start\n");
    fprintf(pfile, "section .text\n");
    fprintf(pfile, "_start:\n");


    for (int i = 0; i < IR_array->size; i++) {
        print_x86_command(&(IR_array->data[i]), pfile);
    }

    fprintf(pfile, "mov rax, 60 \n");
    fprintf(pfile, "mov rdi, 0\n");
    fprintf(pfile, "syscall\n");

    fclose(pfile);
}

int main() {

    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    IR_elements IR_array = {};
    
    make_IR_array(tree, &IR_array);
    dump_IR(&IR_array);

    tree_visualize(tree);
    print_asm_code(tree);
    print_x86_asm(&IR_array);
    return 0;
}

#define MATH_OP_SWITCH_ARG(num)                     \
    case num:                                       \
    set_IR_element(IR_array, operator_class, num);  \
    break;                      


#define CONDITION_CASE(num)                       \
    case num:                                     \
    set_IR_element(IR_array, syntax_class, num);  \
    break;


#define IS_NULL(element)      \
    if (element == NULL) {    \
        return;               \
    }                         

#define ELEM_ARGUMENT(argument, info) IR_array->data[IR_array->size - 1].argument.info



// TODO: make with switch to make it more readable
void make_IR_array(diff_tree_element * element, IR_elements * IR_array) {

    IS_NULL(element);

    if (ELEM_OP_ARG == 2) {

        // TODO: This is not "EQUAL", it's "ASSIGN"
        if (ELEM_OP_NUM == OP_EQUAL) { // NOTE: a strange system

            // TODO: You shouldn't proceed with traversing the tree here,
            //       instead codegenerate for assign specifically:
            //
            // It can be as follows (in stack based processor):
            // compile_expression(instruction_stack, assign->rvalue);
            // register_id reg = assign_empty_register(assign->variable);
            // generate_push(instruction_stack, reg);
            //
            //
            // If you can have arbitrary lvalues (like *(array + b + d) = c):
            //
            // // We consider assignment to look like: *assign->lvalue = assign->rvalue;
            // // and we also consider variables to be pointers to themselves.
            //
            // compile_expression(ctx->instruction_stack, assign->rvalue);
            // if (expression_is_variable(assign->lvalue) && ctx_var_has_designated_reg(ctx, assign->lvalue)) {
            //     register_id reg = ctx_var_get_designated_reg(ctx, assign->variable);
            //     generate_push(instruction_stack, reg);
            // } else {
            //     compile_expression(ctx->instruction_stack, assign->lvalue);
            //     register_id address = ctx_get_free_register(ctx);
            //
            //     generate_pop(instruction_stack, address);
            //     register_id value = ctx_get_free_register(ctx);
            //
            //     generate_pop(instruction_stack, value);
            //     generate_deferencing_mov(instruction_stack, address, value);
            // }

            make_IR_array(element->right, IR_array);
            make_IR_array(element->left, IR_array);
            return;
        } 

        make_IR_array(element->left, IR_array);
        make_IR_array(element->right, IR_array);

        switch (ELEM_OP_NUM) {

            MATH_OP_SWITCH_ARG(OP_ADD);
            MATH_OP_SWITCH_ARG(OP_SUB);
            MATH_OP_SWITCH_ARG(OP_MUL);
            MATH_OP_SWITCH_ARG(OP_DIV);

            END_OF_SWITCH;
        }

        IR_array->data[IR_array->size - 1].countity_of_arg = 2; // TODO: it is costil', fix it, maybe put in set IR

    } else {
        
        if (ELEM_TYPE == value_class) {

            set_IR_element(IR_array, syntax_class, OP_PUSH);
            
            ELEM_ARGUMENT(first_arg, type) = value_class;               //
            ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;     // may be a func

            IR_array->data[IR_array->size - 1].countity_of_arg = 1; // TODO: it is also a temporary solution

        } else if (ELEM_TYPE == variable_class) {

            if (IS_ELEM(element->parent, syntax_class, OP_EQUAL) & !IS_ELEM(element->parent->parent, syntax_class, OP_IF)
                                                             & !IS_ELEM(element->parent->parent, syntax_class, OP_WHILE)) {

                set_IR_element(IR_array, syntax_class, OP_POP);
            
                ELEM_ARGUMENT(first_arg, type) = variable_class;            //
                ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;     // may be put in function

                IR_array->data[IR_array->size - 1].countity_of_arg = 1;
                
            } else {

                set_IR_element(IR_array, syntax_class, OP_PUSH);
            
                ELEM_ARGUMENT(first_arg, type) = variable_class;
                ELEM_ARGUMENT(first_arg, op_number) = (int)ELEM_DOUBLE;
                IR_array->data[IR_array->size - 1].countity_of_arg = 1;
            }

        } else if (ELEM_TYPE == function_class) { // TODO: PLEASE, make call_function and define_function VERY separate, AS separate AS it gets
            if (element->right == NULL) {
                set_IR_element(IR_array, function_class, OP_FUNC);
                IR_array->data[IR_array->size - 1].junk_for_junc = element->value.number; // PIZDEZ
                
            } else {
                int jump_over_func_label = get_free_label(labels_global);

                set_IR_element(IR_array, function_class, OP_FUNC);    // 
                IR_array->data[IR_array->size - 1].countity_of_arg = 1;         // if it is a declaration of func
                                                                                // it is like a marker that it is a declaration

                IR_array->data[IR_array->size - 1].junk_for_junc = element->value.number;
                                                                    
                IR_array->data[IR_array->size - 1].label = jump_over_func_label;

                make_IR_array(element->right, IR_array);

                set_IR_element(IR_array, function_class, OP_LABEL);

                IR_array->data[IR_array->size - 1].label = jump_over_func_label;
            }


        } else {

            if (ELEM_OP_NUM == OP_END) {

                make_IR_array(element->left, IR_array);
                make_IR_array(element->right, IR_array);

            } else if (ELEM_OP_NUM == OP_WHILE) {


                int begin = get_free_label(labels_global);
                int end = get_free_label(labels_global);

                set_IR_element(IR_array, syntax_class, OP_LABEL);
                IR_array->data[IR_array->size - 1].label = begin;

                make_IR_array(element->left->left, IR_array);
                make_IR_array(element->left->right, IR_array);


                switch (element->left->value.operator_info.op_number) {

                    CONDITION_CASE(OP_EQUAL);    // are changed to opposite commands,
                    CONDITION_CASE(OP_MORE);     // because jump happens in opposite case
                    CONDITION_CASE(OP_LESS); 
                    CONDITION_CASE(OP_NEQUAL); 
                           
                    END_OF_SWITCH;
                }

                make_IR_array(element->right, IR_array);

                set_IR_element(IR_array, syntax_class, OP_JUMP);
                IR_array->data[IR_array->size - 1].label = begin;


                set_IR_element(IR_array, syntax_class, OP_LABEL);
                IR_array->data[IR_array->size - 1].label = end;



            } else if (ELEM_OP_NUM == OP_IF ) {

                int end = get_free_label(labels_global);

                make_IR_array(element->left->left, IR_array);
                make_IR_array(element->left->right, IR_array);


                switch (element->left->value.operator_info.op_number) {

                    CONDITION_CASE(OP_EQUAL);    // are changed to opposite commands,
                    CONDITION_CASE(OP_MORE);     // because jump happens in opposite case
                    CONDITION_CASE(OP_LESS); 
                    CONDITION_CASE(OP_NEQUAL); 
                           
                    END_OF_SWITCH;
                }

                IR_array->data[IR_array->size - 1].label = end; // NO

                make_IR_array(element->right, IR_array);

                set_IR_element(IR_array, syntax_class, OP_LABEL);
                IR_array->data[IR_array->size - 1].label = end;

            } else if (ELEM_OP_NUM == OP_PRINT) {

                set_IR_element(IR_array, syntax_class, OP_PRINT);
                ELEM_ARGUMENT(first_arg, type) = variable_class;                        //
                ELEM_ARGUMENT(first_arg, op_number) = element->right->value.number;     // may be put in function

            } else if (ELEM_OP_NUM == OP_RET) {

                set_IR_element(IR_array, syntax_class, OP_RET);

            } else {
                printf("unknown arg in print single command\n");
            }
        }
    }
}
