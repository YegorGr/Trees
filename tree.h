#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef double elem_t;

enum ErrNode {
    IS_OK,
    NO_MEMORY,
    INCORRECT_PARENT,
    ERR_LEFT_CONNECTION,
    ERR_RIGHT_CONNECTION,
    RIGHT_SELF_DESTRUCTION,
    LEFT_SELF_DESTRUCTION,
    THE_SAME_LEFT_RIGHT,
    NULL_PTR,
};

struct node_t
{
    elem_t  data;
    node_t* left;
    node_t* right;
    node_t* parent;
};

ErrNode CreateNode (node_t** new_node, node_t* parent, elem_t data);
ErrNode DeleteNode (node_t* node);

ErrNode LinkRight (node_t* node_p, node_t* node_c);
ErrNode LinkLeft  (node_t* node_p, node_t* node_c);

ErrNode NodeVerify (node_t* node);