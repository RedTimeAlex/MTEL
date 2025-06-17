#include "mtellib/Commands.h"
#include "mtellib/Types.h"
#include <stdlib.h>

#define COMMANDS_SYSTEM_SIZE (*(MTEL_LIB_TYPE_size*)commands_system)
#define COMMANDS_SYSTEM_COMMAND_COUNT (*((MTEL_LIB_TYPE_size*)commands_system+1))
#define COMMANDS_SYSTEM_COMMAND_ARRAY ((struct command* )((MTEL_LIB_TYPE_size*)commands_system+2))

struct command{
	char* name;
	void* data;
	MTEL_LIB_TYPE_command_flags flags;
};

struct MTEL_LIB_command_system* MTEL_LIB_command_system_create(MTEL_LIB_TYPE_size size){
	void* commands_system = malloc(2*sizeof(MTEL_LIB_TYPE_size)+size*sizeof(struct command));
	COMMANDS_SYSTEM_SIZE = size;
	COMMANDS_SYSTEM_COMMAND_COUNT=0;
	return commands_system;
}

void MTEL_LIB_command_system_destruct(struct MTEL_LIB_command_system* commands_system){
	while(COMMANDS_SYSTEM_COMMAND_COUNT){
		free(COMMANDS_SYSTEM_COMMAND_ARRAY[COMMANDS_SYSTEM_COMMAND_COUNT].name);
		COMMANDS_SYSTEM_COMMAND_COUNT--;
	}

	free(commands_system);
}

int MTEL_LIB_command_system_add_command(
		struct MTEL_LIB_command_system* commands_system, 
		const char* const commandName, 
		MTEL_LIB_TYPE_command_flags flags,
		void* data){
	if(!commands_system) return 1;
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

MTEL_LIB_TYPE_command_id MTEL_LIB_command_system_get_type_for_name(const struct MTEL_LIB_command_system* const commands_system, const char* const name){
	if(!commands_system) return 0;
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
	const struct MTEL_LIB_command_system* const commands_system, 
	MTEL_LIB_TYPE_command_id command_id, 
	struct MTEL_LIB_stack* stack,
 	struct MTEL_LIB_type_system* type_system){
	if(!commands_system) return 0;
	if(command_id > COMMANDS_SYSTEM_COMMAND_COUNT) return 1;
	command_id--;

	if(!(COMMANDS_SYSTEM_COMMAND_ARRAY[command_id].flags & MTEL_LIB_COMMAND_FLAG_EXTR)) return 2;
	((void (*)(struct MTEL_LIB_stack*,struct MTEL_LIB_type_system*))COMMANDS_SYSTEM_COMMAND_ARRAY[command_id].data)(stack,type_system);
	return 0;
}
