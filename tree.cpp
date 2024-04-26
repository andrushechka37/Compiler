// #include "../tree.h"
#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


static int set_type_and_value(double value, types_of_node type, diff_tree_element * element);

// TODO(DONE): This are lexems, not operators
lexem_names_numbers_class lexems_names_numbers[OP_COUNT] = {
        {OP_ADD,      "+",       2},
        {OP_SUB,      "-",       2},
        {OP_MUL,      "*",       2},
        {OP_DIV,      "/",       2},
        {OP_SQRT,  "sqrt",       1},
        {OP_SIN,    "sin",       1},
        {OP_COS,    "cos",       1},
        {OP_POW,      "^",       2},
        {OP_FIG_C,    "}",       0},
        {OP_FIG_O,    "{",       0},
        {OP_ROUND_O,  "(",       0},
        {OP_ROUND_C,  ")",       0},
        {OP_IF,      "if",       0},
        {OP_WHILE,"while",       0},
        {OP_MORE,     ">",       0},
        {OP_LESS,     "<",       0},
        {OP_EQUAL,    "=",       2},
        {OP_END,      ";",       0},
        {OP_FUNC,     "$",       0},
        {OP_PRINT,"print",       0},
        {OP_RET,    "ret",       0},
        {OP_NEQUAL,   "!",       0},
        {OP_MOVE,   "move",       2},
};

int is_one_char_symbol(char name) {
    int i = 0;
    while (name != lexems_names_numbers[i].name[0]) {
        i++;
        if (i > OP_COUNT) {
            return 0;
        }
    }
    return lexems_names_numbers[i].number;
}

int is_func_name(char name[]) { // do it faster with hashes instead of strcmp
    int i = 0;
    while (strcmp(lexems_names_numbers[i].name, name)) {
        i++;
        if (i > FUNCS_COUNT) {
            return -1;
        }
    }
    return lexems_names_numbers[i].number;
}

diff_tree_element * node_ctor(double value, types_of_node type, diff_tree_element * left,
                              diff_tree_element * right, diff_tree_element * parent) {

    diff_tree_element * element = (diff_tree_element *) calloc(1, sizeof(diff_tree_element));
    element->type = type;

    switch (type) {
        case value_class:
            ELEM_DOUBLE = value;
            break;

        case operator_class:
            ELEM_OP_NUM = (operations) value;
            element->value.operator_info.arg_quantity = get_op_arg_number((operations) value);
            break;

        case syntax_class:
            ELEM_OP_NUM = (operations) value;
            element->value.operator_info.arg_quantity = get_op_arg_number((operations) value);
            break;
        
        case variable_class:
            ELEM_DOUBLE = value;
            break;
        case function_class:
            ELEM_DOUBLE = value;
            break;
    }

    element->left = left;
    element->right = right;
    element->parent = parent;
    return element;
    
}



const char * get_op_symbol(int op_num) { 
    int i = 0;
    while (op_num != lexems_names_numbers[i].number) i++;      // switch case
    return lexems_names_numbers[i].name;
}





int tree_verify(diff_tree_element * element) {
    if (element == NULL) {
        return 1;
    }
    tree_verify(element->left);

    switch (element->type) {
    case value_class:
        if (element->left != NULL || element->right != NULL) {   // if number node has left and right NULL children
            printf("%p number does not have all nulls", element);
            error_status = 1;
        }
        break;
    case operator_class:
        if (ELEM_OP_ARG == 1) { // if operator has one argument
            if (element->left != NULL || element->right == NULL) {
                printf("%p op does not have all numbers 1", element);
                error_status = 1;
            }
        } else {
            if (element->left == NULL || element->right == NULL) { // if operator has two arguments
                printf("%p op does not have all numbers", element);
                error_status = 1;
            }
        }
    
    default:
        break;
    }

    if (element->left != NULL && element->right != NULL) {   // if parents clild and child's parent are the same
        if(element->left->parent != element || element->right->parent != element) {
        printf("%p - left parent %p - right parent %p - elemen", element->left->parent, element->right->parent, element);
        error_status = 1;
        }
    }
    tree_verify(element->right);
    if (error_status == 1) {
        return 1;
    }
    return 0;
}

bool op_priority(double op1, double op2) {
    if (((int)op1 & OP_PRIORITY_MASK) < ((int)op2 & OP_PRIORITY_MASK)) {
        return 1;
    }
    return 0;
}




void tree_dtor(elem_ptr * root) {
    if (*root == NULL) {     
        return;
    }

    tree_dtor(&(*root)->left);

    (*root)->parent = NULL;
    set_type_and_value(0, (types_of_node)0, *root);

    tree_dtor(&(*root)->right);

    (*root)->right = NULL;
    (*root)->left = NULL;
    free(*root);
    (*root) = NULL;
    return;
}



int get_op_arg_number(operations op) {
    int i = 0;
    while (op != lexems_names_numbers[i].number) {
        if (i > FUNCS_COUNT) {
            return -1;
        }
        i++;
    }
    return lexems_names_numbers[i].arg_quantity;
}

static int set_type_and_value(double value, types_of_node type, diff_tree_element * element) { // null ptr check
    IS_NULL_PTR(element);
    if (type == value_class) {
        element->value.number = value;
    } else if (type == operator_class) {
        ELEM_OP_NUM = (operations)value;
        ELEM_OP_ARG = get_op_arg_number((operations)value);

    }
    element->type = type;
    return 0;
}











static void print_graph_arrows(diff_tree_element * element, FILE * pfile);
static void print_graph_node(diff_tree_element * element, FILE * pfile, int rank);

void print_tree_inorder(diff_tree_element * root) {
    if (root == NULL) {     
        printf("_");
        return;
    }
    printf("(");
    print_tree_inorder(root->left);
    if (root->type == value_class) {
        printf("%.2lf", root->value);
    } else if ((int)root->type == operator_class) {
        //printf("%c", get_op_symbol(root->value));
    }
    
    print_tree_inorder(root->right);
    printf(")");
    return;
}

static void print_graph_arrows(diff_tree_element * element, FILE * pfile) {
    if (element->left != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"#0ae7ff\"];\n", element, element->left);
        print_graph_arrows(element->left, pfile);
    }

    if (element->right != NULL) {
        fprintf(pfile, "\t%d->%d [color = \"#f0ff4f\"];\n", element, element->right);
        print_graph_arrows(element->right, pfile);
    }
    return;
}

static void print_graph_node(diff_tree_element * element, FILE * pfile, int rank) {
    if (hard_visualize == 1) {                              // i know copypast, but i thuink it is not critical,
        fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d,"  // it is for better understanding
                   "label=\"{parent: %p | name: %p | {value: %.2lf | type: %d | arg %d} | {left: %p | right: %p}}\"];\n", 
                   element, rank, element->parent, element, element->value.number, 
                 element->type, element->value.operator_info.arg_quantity, element->left, element->right);
    } else {                                         // ifndef
        if (element->type == value_class) {

            fprintf(pfile, "\t%d[shape=Mrecord,style=filled, fillcolor=\"#7293ba\", rank = %d," 
                   "label=\"%.2lf\"];\n", element, rank, element->value.number);  //obertka // for me it easier without it

        } else if (element->type == operator_class) {

            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#b481f7\", rank = %d," 
                   "label=\"%s\"];\n", element, rank, get_op_symbol(element->value.operator_info.op_number));

        } else if (element->type == variable_class) {

            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#f0ab67\", rank = %d," 
                   "label=\"x\"];\n", element, rank);

        } else if (element->type == syntax_class) {

            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#9487fa\", rank = %d," 
                   "label=\"%s\"];\n", element, rank, get_op_symbol(element->value.operator_info.op_number));

        } else if (element->type == function_class) {

            fprintf(pfile, "\t%d[shape=circle,style=filled, fillcolor=\"#ed64dd\", rank = %d," 
                   "label=\"F - %lg\"];\n", element, rank, element->value.number);
        }

    }
    if (element->left != NULL) {
        print_graph_node(element->left, pfile, ++rank);
    }

    if (element->right != NULL) {
        print_graph_node(element->right, pfile, ++rank);
    }
    return;
}

void tree_visualize(diff_tree_element * element) {
    FILE * pfile = fopen("graph.dot", "w");
    fprintf(pfile, "digraph structs {\n");
    fprintf(pfile, "\trankdir=HR;\n");
    fprintf(pfile, "\tgraph [bgcolor=\"#31353b\"]\n");
    if (hard_visualize == 1) {
        fprintf(pfile, "\tnode[color=\"black\",fontsize=14];\n");
    } else {
        fprintf(pfile, "\tnode[color=\"black\",fontsize=18];\n");
    }

    fprintf(pfile, "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12,  width=0.4];\n\n\n");

    print_graph_node(element, pfile, 1);

    fprintf(pfile, "\n\n\n\n");
    
    print_graph_arrows(element, pfile);

    fprintf(pfile, "\n\n\n}");
    fclose(pfile);
    create_new_graph();
}


void create_new_graph(void) {  // not tmp files, but better than previous hardcode of path
    char command1[command_len] = "dot -Tpng ./graph.dot -o graphs/graph";
    char command2[] = ".png";
    char graph_number_str[2] = {};

    snprintf(graph_number_str, 2,  "%d", graph_number);
    strcat(command1, graph_number_str);
    strcat(command1, command2);
    system(command1);
    graph_number++;
}










// --------------------------------------------------------------------------------------------



