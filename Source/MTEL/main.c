#include "mtellib/Commands.h"
#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"
#include "mtellib/Execute.h"
#include <stdint.h>
#include <stdio.h>

void MTEL_test(struct MTEL_LIB_stack* stack);

void add(struct MTEL_LIB_stack* stack,struct MTEL_LIB_type_system* type_system){
	int a = 0;
	int b = 0;
	MTEL_LIB_stack_pop(stack, &a, type_system);
	MTEL_LIB_stack_pop(stack, &b, type_system);
	a += b;
	MTEL_LIB_stack_push(stack, &a, type_system, MTEL_LIB_type_system_get_type_for_name(type_system,"int32"));
}

void printInt(struct MTEL_LIB_stack* stack,struct MTEL_LIB_type_system* type_system){
	int a = 0;
	MTEL_LIB_stack_pop(stack, &a, type_system);
	printf("int: %d\n", a);
}

int read_token(FILE* file,char* buffer, int buffersize, MTEL_LIB_command_system* command_sytem){
	char c ;
	do{
		c = getc(file);
	}
	while(c == ' ' || c == '\n' || c == '\t' || c == '\r');

	size_t token_size = 0;
	buffersize-=2;
	if(token_size >= buffersize)
	{
		printf("NULL OR SMALL BUFFER");
		return 1;
	}
	do{
		buffer[1 + token_size++] = c;
		c = getc(file);
		if (c == '\\' ){
			c = getc(file);
			buffer[token_size++] = c;
			c = getc(file);
		}
	}
	while(1+token_size < buffersize && c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != EOF);
	
	buffer[1+token_size] = '\0';

	if(buffer[1] == '/'){
		buffer[0] = MTEL_LIB_BYTE_CODE_STR;
		for(int i = 0; i < token_size;i++){
			buffer[1+i] = buffer[2+i];
		}
		buffer[token_size]= MTEL_LIB_BYTE_CODE_END;
	}
	else if('1' <= buffer[1] && buffer[1] <= '9'){
		buffer[0] = MTEL_LIB_BYTE_CODE_INT;
		int res = 0;
		for(int i = 0; i < token_size && '0' <= buffer[1+i] && buffer[1+i] <= '9';i++){
			res *= 10;
			res += (buffer[1+i] - '0');
		}
		*(int*)(buffer+1)=res;
		buffer[6]=MTEL_LIB_BYTE_CODE_END;
	}
	else{
		buffer[0] = MTEL_LIB_BYTE_CODE_COMMAND;	
		*(MTEL_LIB_TYPE_command_id*)(buffer+1) = MTEL_LIB_command_system_get_type_for_name(command_sytem, buffer+1);
		buffer[1+sizeof(MTEL_LIB_TYPE_command_id)] = MTEL_LIB_BYTE_CODE_END;
	}
	if(c==EOF) return -1;
	
	return 0;
}

int main(int argc,char ** argv)
{
	if(argc != 2){
		printf("Usage: %s file.mtel\n",argv[0]);
		return 1;
	}
	//PROGRAMM INIT
	struct MTEL_LIB_type_system* typeSystem = MTEL_LIB_type_system_create(2);
	struct MTEL_LIB_stack* 		 stack  	= MTEL_LIB_stack_create(20);
	struct MTEL_LIB_command_system* command_sytem = MTEL_LIB_command_system_create(2);

	MTEL_LIB_type_system_add_type(typeSystem,"int8",1);
	MTEL_LIB_type_system_add_type(typeSystem,"int32",4);
	MTEL_LIB_command_system_add_command(command_sytem, "add", MTEL_LIB_COMMAND_FLAG_EXTR, &add);
	MTEL_LIB_command_system_add_command(command_sytem, "printInt", MTEL_LIB_COMMAND_FLAG_EXTR, &printInt);

	FILE* file = fopen(argv[1], "r");
	char buffer[128] = {};

	while(!read_token(file, buffer,128, command_sytem)) {
		MTEL_LIB_execute(typeSystem, stack, command_sytem, buffer);
	}

	fclose(file);

	MTEL_LIB_command_system_destruct(command_sytem);
	MTEL_LIB_type_system_destruct(typeSystem);
	MTEL_LIB_stack_destuct(stack);
}
