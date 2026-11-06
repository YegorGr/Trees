#include "tree.h"

int main ()
{
    node_t* node1 = nullptr;
    CreateNode(&node1, 0, 8.0);
}

ErrNode CreateNode (node_t** new_node, node_t* parent, elem_t data)
{
    assert(new_node);
    
    node_t* ptr = (node_t*) calloc (1, sizeof(node_t));
    if (ptr == nullptr)
        return NO_MEMORY;
    
    ptr->parent = parent;
    ptr->data = data;
    
    ErrNode err = NodeVerify(ptr);
    if (err != IS_OK) 
    {
        //TreeDump();
        free(ptr);
        return err;
    }

    *new_node = ptr;

    return IS_OK;
}

ErrNode NodeVerify (node_t* node)
{
    assert(node);

    if (node->parent < 0)
        return INCORRECT_PARENT;
    
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

    if (node->right == node->left)
        return THE_SAME_LEFT_RIGHT;

    return IS_OK;
}

ErrNode DeleteNode (node_t* node)
{
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

    node->data   = NAN; 
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = nullptr;

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

    return IS_OK;
}

ErrNode LinkLeft (node_t* node_p, node_t* node_c)
{
    ErrNode err = IS_OK;
    if ((err = NodeVerify(node_p)) != IS_OK || (err = NodeVerify(node_c)) != IS_OK)
        return err;
    
    node_p->left   = node_c;
    node_c->parent = node_p;

    return IS_OK;
}

