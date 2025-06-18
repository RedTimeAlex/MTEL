#include "mtellib/Commands.h"
#include "mtellib/Types.h"
#include <stdlib.h>

#define COMMANDS_SYSTEM_SIZE (*(MTEL_LIB_TYPE_size*)context->command_system)
#define COMMANDS_SYSTEM_COMMAND_COUNT (*((MTEL_LIB_TYPE_size*)context->command_system+1))
#define COMMANDS_SYSTEM_COMMAND_ARRAY ((struct command* )((MTEL_LIB_TYPE_size*)context->command_system+2))

struct command{
	char* name;
	void* data;
	MTEL_LIB_TYPE_command_flags flags;
};

int MTEL_LIB_command_system_create(MTEL_LIB_context* context,MTEL_LIB_TYPE_size size){
	if(!context) return 1;
	context->command_system = malloc(2*sizeof(MTEL_LIB_TYPE_size)+size*sizeof(struct command));
	if(!context->command_system) return 2;
	COMMANDS_SYSTEM_SIZE = size;
	COMMANDS_SYSTEM_COMMAND_COUNT=0;
	return 0;
}

void MTEL_LIB_command_system_destruct(MTEL_LIB_context* context){
	if(!context) return;
	while(COMMANDS_SYSTEM_COMMAND_COUNT){
		free(COMMANDS_SYSTEM_COMMAND_ARRAY[COMMANDS_SYSTEM_COMMAND_COUNT].name);
		COMMANDS_SYSTEM_COMMAND_COUNT--;
	}

	free(context->command_system);
}

int MTEL_LIB_command_system_add_command(
		MTEL_LIB_context* context,
		const char* const commandName, 
		MTEL_LIB_TYPE_command_flags flags,
		void* data){
	if(!context) return 1;
	if(!commandName)	 return 2;
	if(flags!=MTEL_LIB_COMMAND_FLAG_EXTR) return 3;
	if(COMMANDS_SYSTEM_COMMAND_COUNT==COMMANDS_SYSTEM_SIZE) return 4;

	MTEL_LIB_TYPE_size nameSize = 0;
	while(commandName[nameSize]!='\0'){
		nameSize++;
	}
	char* name = malloc(nameSize+1);
	if(!name) return 5;
	for(MTEL_LIB_TYPE_size i = 0; i <= nameSize;i++){
		name[i]=commandName[i];
	}
	name[nameSize+1]='\0';

	COMMANDS_SYSTEM_COMMAND_ARRAY[COMMANDS_SYSTEM_COMMAND_COUNT]= (struct command){name,data,flags};
	COMMANDS_SYSTEM_COMMAND_COUNT++;

	return 0;
}

MTEL_LIB_TYPE_command_id MTEL_LIB_command_system_get_type_for_name(MTEL_LIB_context* context, const char* const name){
	if(!context) return 0;
	if(!name) return 0;

	for(MTEL_LIB_TYPE_size i = 0; i < COMMANDS_SYSTEM_COMMAND_COUNT;i++){
		MTEL_LIB_TYPE_size cmp = 0;	
		while(COMMANDS_SYSTEM_COMMAND_ARRAY[i].name[cmp]!='\0' && name[cmp]!='\0'){
			if(COMMANDS_SYSTEM_COMMAND_ARRAY[i].name[cmp]!=name[cmp]){
				cmp = 0; break;
			}
			cmp++;
		}

		if(cmp) return ++i;
	}
	return 0;
}

int MTEL_LIB_command_call(
	MTEL_LIB_context* context,
	MTEL_LIB_TYPE_command_id command_id){
	if(!context) return 0;
	if(command_id > COMMANDS_SYSTEM_COMMAND_COUNT) return 1;
	command_id--;

	if(!(COMMANDS_SYSTEM_COMMAND_ARRAY[command_id].flags & MTEL_LIB_COMMAND_FLAG_EXTR)) return 2;
	((void (*)(MTEL_LIB_context*))COMMANDS_SYSTEM_COMMAND_ARRAY[command_id].data)(context);
	return 0;
}
