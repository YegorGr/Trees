#include "tree.h"

int main (int argc, const char* argv[])
{
    const char* filename = nullptr;
    tree_t      tree     = {};

    FILE* in = OpenFile(argc, argv, &filename);

    if (in != nullptr)
    {
        tree.root = LoadTreeFromFile(in);
        fclose(in);
        tree.size = SubTree(tree.root, &tree);
    }

    if (tree.root == nullptr)
    {
        node_t* root = nullptr;
        CreateTree(&tree, &root, "Null tree");
    }

    ModeSelect(&tree);

    FILE* out = fopen(filename, "w");
    if (out != nullptr)
    {
        PrePrint(tree.root, out);
        fclose(out);
    }

    DeleteNode(tree.root);
}


