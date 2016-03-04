#define STACK_MAX 10000
#include "stack.h"
struct Stack 
{
    char * data[STACK_MAX];
    int size;
};



void Stack_Init(Stack *S)
{
    S->size = 0;
}

char* Stack_Top(Stack *S)
{
    if (S->size == 0) 
    {
        fprintf(stderr, "Error: stack empty\n");
        return NULL;
    } 

    return S->data[S->size-1];
}

void Stack_Push(Stack *S, char* d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
    else
        fprintf(stderr, "Error: stack full\n");
}

void Stack_Pop(Stack *S)
{
    if (S->size == 0)
        fprintf(stderr, "Error: stack empty\n");
    else
    {
        S->size--;
    }
}