#include "mtellib/Commands.h"
#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"
#include "mtellib/Execute.h"
#include <stdint.h>
#include <stdio.h>

void MTEL_test(struct MTEL_LIB_stack* stack);

void add(MTEL_LIB_context* context){
	int a = 0;
	int b = 0;
	MTEL_LIB_stack_pop(context, &a);
	MTEL_LIB_stack_pop(context, &b);
	a += b;
	MTEL_LIB_stack_push(context, &a, MTEL_LIB_type_system_get_type_for_name(context,"int32"));
}

void printInt(MTEL_LIB_context* context){
	int a = 0;
	MTEL_LIB_stack_pop(context, &a);
	printf("int: %d\n", a);
}

int read_token(FILE* file,char* buffer, int buffersize, MTEL_LIB_context* context){
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
		*(MTEL_LIB_TYPE_command_id*)(buffer+1) = MTEL_LIB_command_system_get_type_for_name(context, buffer+1);
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
	MTEL_LIB_context context = {};
	
	MTEL_LIB_type_system_create(&context,2);
	MTEL_LIB_stack_create(&context, 128);
	MTEL_LIB_command_system_create(&context,2);

	MTEL_LIB_type_system_add_type(&context,"int8",1);
	MTEL_LIB_type_system_add_type(&context,"int32",4);
	MTEL_LIB_command_system_add_command(&context, "add", MTEL_LIB_COMMAND_FLAG_EXTR, &add);
	MTEL_LIB_command_system_add_command(&context, "printInt", MTEL_LIB_COMMAND_FLAG_EXTR, &printInt);

	FILE* file = fopen(argv[1], "r");
	char buffer[128] = {};

	while(!read_token(file, buffer,128, &context)) {
		MTEL_LIB_execute(&context, buffer);
	}

	fclose(file);

	MTEL_LIB_command_system_destruct(&context);
	MTEL_LIB_type_system_destruct(&context);
	MTEL_LIB_stack_destuct(&context);
}
