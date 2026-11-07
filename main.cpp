#include "tree.h"

int main ()
{
    node_t* node1 = nullptr;
    node_t* node2 = nullptr;
    node_t* node3 = nullptr;
    node_t* node4 = nullptr;

    tree_t tree1 = {};
   
    CreateTree(&tree1, &node1, 50);

    CreateNode(&node2, 60);
    CreateNode(&node3, 40);
    CreateNode(&node4, 70);

    LinkLeft(node1, node3);
    LinkRight(node1, node2);
    LinkRight(node2, node4);

    DumpTree(&tree1);
}


