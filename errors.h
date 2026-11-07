#include <stdio.h>

#define ERR_NODE_LIST(X)      \
    X(IS_OK)                  \
    X(NO_MEMORY)              \
    X(INCORRECT_PARENT)       \
    X(ERR_LEFT_CONNECTION)    \
    X(ERR_RIGHT_CONNECTION)   \
    X(RIGHT_SELF_DESTRUCTION) \
    X(LEFT_SELF_DESTRUCTION)  \
    X(THE_SAME_LEFT_RIGHT)    \
    X(NULL_PTR)


#define GENERATE_ENUM(name) name, 

enum ErrNode 
{
    ERR_NODE_LIST(GENERATE_ENUM)
};

#undef GENERATE_ENUM


extern const char* ErrString [];