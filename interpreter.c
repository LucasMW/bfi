// Brainfuck Interpreter
// by Lucas Menezes 


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEFAULT_SIZE 100000000 // less than 1 MB
#define DEBUG 1

// based on  http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html
#define STACK_MAX 100
struct Stack 
{
    char*     data[STACK_MAX];
    int     size;
};
typedef struct Stack Stack;


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
void printMemory(char *memory,char mode)
{
	int i;
	printf("Memory\n");
	printf("[");

	for (i =0 ; i < DEFAULT_SIZE ; i++)
	{
		if(mode == 'c')
			printf("%c ", memory[i]);
		else
			printf("%d ",memory[i]);
	}
	printf("]\n");
}
void execute(char* program)
{
	Stack* loopStack;
	char instructions[] = {'>','<','+','-','.',',','[',']'};
	char* ptr; 
	char c; 
	char *loopStart = NULL;
	int internalLoopCount = 0;
	char* memory = (char*) malloc(DEFAULT_SIZE); // memory of program
	ptr = memory;
	Stack_Init(loopStack);
	//printf("executing\n");
	//printf("%s\n",program);
	//printf("c: %c\n",*program);
	
	while(*program)
	{
		//printf("while\t");
		c = *program;
		//printf("%x %c ptr: %x {%d}\n",program,c,ptr,*ptr);
		if (c == instructions[0]) 
		{
			++ptr;
		}
		else if (c == instructions[1])
		{
			--ptr;
		}
		else if (c == instructions[2])
		{
			++*ptr;
		}
		else if (c == instructions[3])
		{
			--*ptr;
		}
		else if (c == instructions[4])
		{
			putchar(*ptr);
		}
		else if (c == instructions[5])
		{
			*ptr=getchar();
		}
		else if (c == instructions[6])
		{
				// will enter while
			if(*ptr)
			{
				//printf("entered [ ");
				loopStart = program;
				//printf("loopStart: %x \n", loopStart);
				Stack_Push(loopStack,loopStart);
			}

			else
			{
					// skip while sequence
				//printf("must skip [ \n");
				program++;
				internalLoopCount = 0;
				while(!(*program == ']' && internalLoopCount == 0)) // while not match stop requirements
				{
					if(*program == '[')
					{
						internalLoopCount++;
					}
					else if(*program == ']')
					{
						internalLoopCount--;
					}
					program++;
					//printf("%c %d\n", *program, internalLoopCount);
					//sleep(1);
				}
			}

		}
		else if (c ==instructions[7])
		{
				// repeats loop
			if(*ptr)
			{
				//printf("shall repeat\n");
				loopStart = Stack_Top(loopStack);
				program = loopStart; 
			}
			else
			{
				//printf("end of loop\n");
				Stack_Pop(loopStack); // out of this loop
			}
		}
		else
		{

		}
		program++;
	}
	//printMemory(memory,'d');
	free(memory);
}

int errorMessage(int argc, char** argv)
{
	printf("Error!\nProgram usage is: \nbfi sourcecode\n");
	return -1;
}
char * readFile(char* path)
{
	int size;
	FILE * input;
	char * fileString;
	char c;
	int i;
	input = fopen(path,"rt");
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);
	//printf("opened\n");
	fileString = (char*)malloc(size+1);
	i = 0;
	while(fscanf(input,"%c",&c) != EOF)
	{
		//printf("%c",c);
		fileString[i] = c;
		i++;
	}
	fileString[size] = '\0';
	//printf("will close\n");
	fclose(input); 
	return fileString;
}
int main (int argc, char** argv)
{
	char * program;
	
	//expects bfi filepath
	if(argc == 2)
	{
		//printf("will read\n");
		program = readFile(argv[1]);
		execute(program);
		free(program);
		return 0;
	}
	else
	{
		return errorMessage(argc, argv);
	} 
	
}
