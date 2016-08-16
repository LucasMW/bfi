// Brainfuck Interpreter
// by Lucas Menezes 


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack.h"
#define DEFAULT_SIZE 100000000 // less than 100 MB
//#define DEBUG

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

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
void printAreaNearPtr(char* ptr,char* memory,int offset)
{

	int x = ptr - memory;
	int i;
	if(x > 0 && x < offset)
	{
		for( i = 0; i < 2* offset; i++)
		{
			printf("|%d",*(ptr+i));
			if(i==0)
				printf("*");
		}
	}
	else if (x < 0 && x*x < offset*offset)
	{
		for( i = -2 * offset; i < 0; i++)
		{
			printf("|%d",*(ptr+i));
			if(i==0)
				printf("*");
		}
	}
	else
	{
		for( i = -offset; i < offset; i++   )
		{
			printf("|%d",*(ptr+i));
			if(i==0)
				printf("*");
		}
	}
	printf("|\r");
}

void execute(char* program)
{
	Stack* loopStack;
	char instructions[] = {'>','<','+','-','.',',','[',']'};
	char* ptr; 
	char c,*cp; 
	int i;
	char *loopStart = NULL;
	int internalLoopCount = 0;
	char* memory = (char*) calloc(sizeof(char),DEFAULT_SIZE); // memory of program must be zeroed.
	ptr = memory;
	Stack_Init(&loopStack);
	
	
	while(*program)
	{
		c = *program;
		//DEBUG_PRINT(("%x\t%c\tptr:\t%x\t{%d}\n",program,c,ptr,*ptr));
		//DEBUG_PRINT(("current instruction \'%c\'\t%d\n",c,c));
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
				loopStart = program;
				Stack_Push(loopStack,loopStart);
			}

			else
			{
				// skip while sequence
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
				}
			}

		}
		else if (c ==instructions[7])
		{
			// repeats loop
			if(*ptr)
			{
				loopStart = Stack_Top(loopStack);
				program = loopStart; 
			}
			else
			{
				Stack_Pop(loopStack); // out of this loop
			}
		}
		else
		{

		}
		program++;
	}
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
	fileString = (char*)malloc(size+1);
	i = 0;
	while(fscanf(input,"%c",&c) != EOF)
	{
		fileString[i] = c;
		i++;
	}
	fileString[size] = '\0';
	fclose(input);
	DEBUG_PRINT(("%s\n",fileString)); 
	return fileString;
}
int main (int argc, char** argv)
{
	char * program;
	
	//expects bfi filepath
	if(argc == 2)
	{
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
