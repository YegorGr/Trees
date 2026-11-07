#include "tree.h"

ErrNode CreateNode (node_t** new_node, elem_t data)
{
    assert(new_node);

    node_t* ptr = (node_t*) calloc (1, sizeof(node_t));
    if (ptr == nullptr)
        return NO_MEMORY;
    
    ptr->data = data;
        
    ErrNode err = NodeVerify(ptr);
    if (err != IS_OK) 
    {
        DumpTree(ptr->tree);
        free(ptr);
        return err;
    }

    *new_node = ptr;
    
    return IS_OK;
}

ErrNode NodeVerify (node_t* node)
{
    assert(node);
    
    if (node->left != nullptr)
    {
        if (node->left->parent != node)
            return ERR_LEFT_CONNECTION;
    }

    if (node->right != nullptr)
    {
        if (node->right->parent != node)
            return ERR_RIGHT_CONNECTION;
    }

    if (node->left == node)
        return LEFT_SELF_DESTRUCTION;
    
    if (node->right == node)
        return RIGHT_SELF_DESTRUCTION;

    if (node->left != nullptr && node->right == node->left)
        return THE_SAME_LEFT_RIGHT;

    return IS_OK;
}

ErrNode DeleteNode (node_t* node)
{
    if (node == nullptr)
        return NULL_PTR;
    
    ErrNode err = IS_OK;
    if ((err = NodeVerify(node)) != IS_OK)
        return err;
    
    if (node->left != nullptr)
        DeleteNode(node->left);
    
    if (node->right != nullptr)
        DeleteNode(node->right);

    if (node->parent != nullptr)
    {
        if (node->parent->left == node)
            node->parent->left = nullptr;

        if (node->parent->right == node)
            node->parent->right = nullptr;
    }
    else 
    {
        if (node->tree != nullptr && node->tree->root == node)
            node->tree->root = nullptr;
    }

    node->data   = NAN; 
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = nullptr;

    if (node->tree != nullptr) 
        node->tree->size--;
    
    free(node);

    return IS_OK;
}

ErrNode LinkRight (node_t* node_p, node_t* node_c)
{
    ErrNode err = IS_OK;
    if ((err = NodeVerify(node_p)) != IS_OK || (err = NodeVerify(node_c)) != IS_OK)
        return err;
    
    node_p->right  = node_c;
    node_c->parent = node_p;

    node_c->tree = node_p->tree;

    if (node_p->tree != nullptr)
        node_p->tree->size++;

    return IS_OK;
}

ErrNode LinkLeft (node_t* node_p, node_t* node_c)
{
    ErrNode err = IS_OK;
    if ((err = NodeVerify(node_p)) != IS_OK || (err = NodeVerify(node_c)) != IS_OK)
        return err;
    
    node_p->left   = node_c;
    node_c->parent = node_p;

    node_c->tree = node_p->tree;

    if (node_p->tree != nullptr)
        node_p->tree->size++;

    return IS_OK;
}

ErrNode CreateTree (tree_t* tree, node_t** new_node, elem_t first_elem)
{
    if (tree == nullptr || new_node == nullptr)
        return NULL_PTR;

    tree->size = 0;
    tree->root = nullptr;

    ErrNode err = IS_OK;
    if ((err = CreateNode(new_node, first_elem)) != IS_OK)
        return err;
    
    tree->root = *new_node;
    tree->size = 1;

    return IS_OK;
}

void DumpTree (tree_t* tree)
{
    if (tree == nullptr) return;

    FILE* dot = fopen("dump.dot", "w");
    if (dot == nullptr) return;

    fprintf(dot, "digraph Tree {\n");

    fprintf(dot, "    nodesep=0.5;\n");
    fprintf(dot, "    ranksep=0.8;\n");

    if (tree->root != nullptr)
        DumpNode(tree->root, dot);

    fprintf(dot, "}\n");
    fclose(dot);

    system("dot -Tpng dump.dot -o dump.png");
}

void DumpNode (node_t* node, FILE* dot)
{
    if (node == nullptr || dot == nullptr) return;

    fprintf(dot, "    node%p [shape=Mrecord, label=\"{ %.2lf | 0x%p | { <left> 0x%p | <right> 0x%p } }\"];\n", 
            node, node->data, node, node->left, node->right);

    if (node->left != nullptr)
    {
        fprintf(dot, "    node%p:left -> node%p [color=\"blue\"];\n", node, node->left);
        DumpNode(node->left, dot);
    }

    if (node->right != nullptr)
    {
        fprintf(dot, "    node%p:right -> node%p [color=\"red\"];\n", node, node->right);
        DumpNode(node->right, dot);
    }

    if (node->left != nullptr && node->right != nullptr)
         fprintf(dot, "    { rank=same; node%p; node%p; }\n", node->left, node->right);
 }