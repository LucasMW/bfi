#include <stdio.h>
#include <stdlib.h>
typedef struct Stack Stack;

void Stack_Init(Stack **S);

char* Stack_Top(Stack *S);


void Stack_Push(Stack *S, char* d);

void Stack_Pop(Stack *S);
