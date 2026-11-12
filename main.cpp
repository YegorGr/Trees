#include "tree.h"

int main ()
{
    const char* filename = "base.txt";
    tree_t tree = {};

    FILE* in = fopen(filename, "r");
    if (in != nullptr)
    {
        tree.root = LoadTreeFromFile(in);
        fclose(in);
        tree.size = SubTree(tree.root, &tree);
    }

    if (tree.root == nullptr)
    {
        node_t* root = nullptr;
        CreateTree(&tree, &root, "nothing");
    }

    bool is_running = true;
    while (is_running)
    {
        printf("\n1 - play\n2 - show base\n0 - exit\n> ");

        char buf[MAX_LEN_STR] = {};
        if (!SaveRead(buf, sizeof buf)) break;

        switch (atoi(buf))
        {
            case 1:  PlayAkinator(&tree); break;
            case 2:  DumpTree(&tree);     break;
            case 0:  is_running = false;  break;
            default: printf("No such item.\n");
        }
    }

    FILE* out = fopen(filename, "w");
    if (out != nullptr)
    {
        PrePrint(tree.root, out);
        fclose(out);
    }

    DeleteNode(tree.root);
}


