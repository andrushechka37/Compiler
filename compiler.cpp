
#include "tree.h"
#include <cstdio>
#include <cstring>
#include "read_to_tree.h"
#include "compiler.h"




int main() {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();
    tree_visualize(tree);
    print_asm_code(tree);
    return 0;
}