#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"

const int MAX_LEN_STR = 1024;

typedef char* elem_t;

struct tree_t;

struct node_t
{
    elem_t  data;
    node_t* left;
    node_t* right;
    node_t* parent;

    tree_t* tree;
};

struct tree_t 
{
    node_t* root;
    size_t  size;
};

ErrNode CreateNode (node_t** new_node, elem_t data);
ErrNode DeleteNode (node_t* node);

ErrNode LinkRight  (node_t* node_p, node_t* node_c);
ErrNode LinkLeft   (node_t* node_p, node_t* node_c);

ErrNode NodeVerify (node_t* node);

ErrNode CreateTree (tree_t* tree, node_t** new_node, elem_t first_elem);

void DumpTree      (tree_t* tree);
void DumpNode      (node_t* node, FILE* dot);