#include "tree.h"

#define GENERATE_STRING(name) #name,

const char* ErrString [] = 
{
    ERR_NODE_LIST(GENERATE_STRING)
};

#undef GENERATE_STRING

ErrNode CreateNode (node_t** new_node, const char* data)
{
    if (new_node == nullptr || data == nullptr)
        return NULL_PTR;
    
    node_t* ptr = (node_t*) calloc (1, sizeof(node_t));
    if (ptr == nullptr)
        return NO_MEMORY;
    
    ptr->data = strdup(data);
        
    if (ptr->data == nullptr)
    {
        free(ptr);
        return NO_MEMORY;
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

    if (node->tree != nullptr)
       node->tree->size--;

    free(node->data);
    free(node);

    node->data   = nullptr; 
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = nullptr;

   return IS_OK;
}

ErrNode LinkRight (node_t* node_p, node_t* node_c)
{
    if (node_p->right != nullptr || node_c->parent != nullptr)
        return SLOT_BUSY;

    ErrNode err = IS_OK;
    if ((err = NodeVerify(node_p)) != IS_OK || (err = NodeVerify(node_c)) != IS_OK)
        return err;
    
    node_p->right  = node_c;
    node_c->parent = node_p;

    return IS_OK;
}

ErrNode LinkLeft (node_t* node_p, node_t* node_c)
{
    if (node_p->left != nullptr || node_c->parent != nullptr)
        return SLOT_BUSY;

    ErrNode err = IS_OK;
    if ((err = NodeVerify(node_p)) != IS_OK || (err = NodeVerify(node_c)) != IS_OK)
        return err;
    
    node_p->left   = node_c;
    node_c->parent = node_p;

    return IS_OK;
}

ErrNode CreateTree (tree_t* tree, node_t** new_node, const char* first_elem)
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
    (*new_node)->tree = tree;

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

    ErrNode err = NodeVerify(node);

    fprintf(dot, "    node%p [shape=Mrecord, "
                 "label=\"{ { Err: %s | Data: %s } | Ptr: %p | { <left> Ptr(L): %p | <right> Ptr(R): %p } }\"];\n", 
            node, ErrString[err], node->data, node, node->left, node->right);

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

size_t SubTree (node_t* node, tree_t* tree)
{
    if (node == nullptr)
        return 0;
        
    node->tree = tree;

    size_t left_count  = SubTree(node->left, tree);
    size_t right_count = SubTree(node->right, tree);

    return 1 + left_count + right_count; 
}

void PrePrint (node_t* node, FILE* out)
{
    if (node == nullptr)
    {
        fprintf(out, " 0 ");
        return;
    }

    fprintf(out, "(\"%s\" ", node->data);
    PrePrint(node->left, out);
    PrePrint(node->right, out);
    fprintf(out, ") ");
}

void SkipSpaces(FILE* in) 
{
    int symbol = 0;

    while ((symbol = fgetc(in)) != EOF && isspace(symbol)) 
        ;
    
    if (symbol != EOF) 
        ungetc(symbol, in); 
}

node_t* LoadTreeFromFile(FILE* in)
{
    SkipSpaces(in);

    int ch = fgetc(in);
    if (ch == EOF)
        return nullptr;

    if (ch == '0')
        return nullptr;

    if (ch == '(')
    {
        char buffer[MAX_LEN_STR] = {};

        fscanf(in, " \"%[^\"]\" ", buffer);

        node_t* new_node = nullptr;
        CreateNode(&new_node, buffer);

        new_node->left = LoadTreeFromFile(in);
        if (new_node->left != nullptr) 
            new_node->left->parent = new_node;
        
        new_node->right = LoadTreeFromFile(in);
        if (new_node->right != nullptr) 
            new_node->right->parent = new_node;;

        SkipSpaces(in);
        ch = fgetc(in);
        if (ch != ')')
            printf("Error to read: waited ')'\n");

        return new_node;
    }

    return nullptr;
}

bool SaveRead (char* buf, size_t size)
{
    if (fgets(buf, size, stdin) == nullptr)
        return false;

    buf[strcspn(buf, "\n")] = '\0';

    return true;
}

int YesOrNo (const char* question)
{
    char buf[MAX_LEN_STR] = {};

    while (true)
    {
        printf("%s (yes/no): ", question);
        if (!SaveRead(buf, sizeof buf))
            return -1;

        if (strcasecmp(buf, "yes") == 0) return 1;
        if (strcasecmp(buf, "no")  == 0) return 0;

        printf("Only yes or no.\n");
    }
}

void PlayAkinator(tree_t* tree) 
{
    if (tree == nullptr || tree->root == nullptr) 
    {
        printf("The data-base is null!\n");
        return;
    }

    node_t* node = tree->root;

    while (node->left != nullptr || node->right != nullptr) 
    {
        int ans = YesOrNo(node->data);  

        if (ans == 1) // right is true
        {
            if (node->right) 
                node = node->right;
        } 
        else if (ans == 0)
        {
            if (node->left) 
                node = node->left;
        }
        else 
        {
            printf("Game over\n");
            return;
        }
    }

    char question[2 * MAX_LEN_STR] = {};
    snprintf(question, sizeof(question), "Is this %s?", node->data);
    int ans = YesOrNo(question);

    if (ans == 1) 
        printf("Eboy!\n");

    else if (ans == 0) 
    {
        printf("Fuck! BUT WHO WAS IT?\n");
        AddNewObject(node);
    }
    else
        printf("Game over!\n");

}

void AddNewObject(node_t* node) 
{
    if (node == nullptr) 
        return;

    char new_object[MAX_LEN_STR]   = {};
    char new_question[MAX_LEN_STR] = {};
    
    ClearBuffer();

    printf("Write down who you’re thinking of: ");
    scanf("%63[^\n]", new_object);
    
    ClearBuffer();

    printf("In contrast \"%s\", \"%s\"... ", node->data, new_object);
    scanf("%63[^\n]", new_question);

    node_t* node_old = nullptr;
    node_t* node_new = nullptr;
    
    CreateNode(&node_old, node->data);
    CreateNode(&node_new, new_object);

    free(node->data);    
    node->data = strdup(new_question);

    LinkRight(node, node_new);   
    LinkLeft(node, node_old);   

    printf("Thanks!\n");
}

void ClearBuffer (void)
{
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    return;
}

 FILE* OpenFileRead (int argc, const char** argv, const char** filename)
{ 
    assert(filename);

    *filename = (argc >= 2) ? argv[1] : DEFAULT_BASE;

    printf("Loading base: %s\n", *filename);

    FILE* in = fopen(*filename, "r");
    if (in == nullptr)
        return nullptr;

    return in;
}

void ModeSelect (tree_t* tree)
{
    bool is_running = true;
    while (is_running)
    {
        printf("\n1 - play akinator\n2 - show base\n0 - exit\n> ");

        char buf[MAX_LEN_STR] = {};

        if (!SaveRead(buf, sizeof(buf))) 
            break;

        switch (buf[0])
        {
            case MENU_PLAY:  PlayAkinator(tree); break;
            case MENU_SHOW:  DumpTree(tree);     break;
            case MENU_EXIT:  is_running = false;  break;

            default: printf("No such item.\n");
        }
    }
}