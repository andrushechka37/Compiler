
#include "tree.h"
#include <cstdio>
#include <cstring>


static bool check_symbol(char symbol, FILE * pfile);
int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent);
static void set_type_value(diff_tree_element * element, double number, types_of_node type);

const int LABELS_QUANTITY = 10;
variables_info variables_table;
int labels_global[LABELS_QUANTITY] = {};


static int take_free_label(int labels[], int number_of_taken_label) {
    labels[number_of_taken_label] = 1;
    return 0;
}

static int get_free_label(int labels[]) {


    for (int i = 0; i < LABELS_QUANTITY; i++) {

        if (labels[i] == 0) {
            labels[i] = 1;
            return i;
        }
    }

    printf("out of range of labels!!!\n");
    return 999;
}

static bool check_symbol(char symbol, FILE * pfile) {

    bool is_found = 1;
    char check_char = getc(pfile);

    if (check_char != symbol) {                           
        ungetc(check_char, pfile);        
        is_found = 0;                                         
    } 

    check_char = getc(pfile); 

    if (check_char != '\n') {                            
        ungetc(check_char, pfile);          
    }
    
    return is_found;
}

#define VAR_NUM (variables_table.size)

// was taken from frontend, think about it, where to put it
static int put_name_to_table(char name[]) {

    int i = 0;
    while (i < variables_table.size) {

        if (strcmp(name, variables_table.table[i].name) == 0) {
            return i;
        }
        i++;
    }

    strcpy(variables_table.table[VAR_NUM].name, name); // not safe
    variables_table.table[VAR_NUM].value = VAR_NUM;

    VAR_NUM++;
    
    if (VAR_NUM > SYMBOL_TABLE_MAX_CAPACITY) {
        printf("!!!!!!!!!!!!!!!!!!too much variables");
    }
    
    return VAR_NUM - 1;
}

diff_tree_element * read_tree() {

    FILE * in_file = fopen("in_program.txt", "r");
    if (in_file == NULL) {
        printf("null ptr");
        return NULL;
    }

    diff_tree_element * element = NULL;

    build_tree(&element, in_file, &element);

    fclose(in_file);
    return element;
}

#define LEFT &((*element)->left)
#define RIGHT &((*element)->right)

int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent) {

    if (check_symbol('(', in_file) == 1) {

        *element = node_ctor(0, zero_class, NULL, NULL, *parent);

        build_tree(LEFT, in_file, element);

        double value = 0;                 

        if (fscanf(in_file, "%lf", &value) == 1) {

            set_type_value(*element, value, value_class);

        } else {

            char op[OP_NAME_LEN] = {};

            if (check_symbol('$', in_file) == 1) {  // skip $ if it is
            
                fscanf(in_file, "%[^_(]s", &op);
                int num = put_name_to_table(op);
                set_type_value(*element, num, function_class);
                take_free_label(labels_global, num);

            } else {
                
                fscanf(in_file, "%[^_(]s", &op);

                if (is_func_name(op) != -1) {

                    SET_RIGHT_TYPE_VALUE(is_func_name(op));

                } else {

                    int num = put_name_to_table(op);

                    set_type_value(*element, num, variable_class);
                }
            }
        }

        build_tree(RIGHT, in_file, element);

        if (check_symbol(')', in_file) == 0) {      
            return 1; 
        }   

    } else if (check_symbol(NIL, in_file) == 1) {
        return 0;
    }
}

static void set_type_value(diff_tree_element * element, double number, types_of_node type) {

    if (type == variable_class || type == value_class || type == function_class) {
        element->value.number = number;
    } else {
        element->value.operator_info.op_number = (operations)number;
        element->value.operator_info.arg_quantity = get_op_arg_number(element->value.operator_info.op_number);
    }

    element->type = type;
}



int main() {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    tree_visualize(tree);
    return 0;
}