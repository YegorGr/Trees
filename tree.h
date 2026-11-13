#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "errors.h"

const int MAX_LEN_STR = 64;
const char* const DEFAULT_BASE = "base.txt";

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

enum Menu
{
    MENU_EXIT = '0',
    MENU_PLAY = '1',
    MENU_SHOW = '2',
    MENU_BAD  = '-1',
};

ErrNode CreateNode (node_t** new_node, const char* data);
ErrNode DeleteNode (node_t* node);

ErrNode LinkRight  (node_t* node_p, node_t* node_c);
ErrNode LinkLeft   (node_t* node_p, node_t* node_c);

ErrNode NodeVerify (node_t* node);

ErrNode CreateTree (tree_t* tree, node_t** new_node, const char* first_elem);

void    DumpTree   (tree_t* tree);
void    DumpNode   (node_t* node, FILE* dot);

void    ClearBuffer(void);

size_t  SubTree    (node_t* node, tree_t* tree);
void    PrePrint   (node_t* node, FILE* out);
void    SkipSpaces (FILE* in);
bool    SaveRead   (char* buf, size_t size);
int     YesOrNo    (const char* question);

node_t* LoadTreeFromFile (FILE* in);
void    PlayAkinator     (tree_t* tree);
void    AddNewObject     (node_t* node);

FILE*   OpenFile         (int argc, const char** argv, const char** filename);
void    ModeSelect       (tree_t* treee);






