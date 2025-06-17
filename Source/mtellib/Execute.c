#include "mtellib/Execute.h"
#include "mtellib/Commands.h"
#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"

int MTEL_LIB_execute(MTEL_LIB_type_system *type_system, MTEL_LIB_stack *stack, MTEL_LIB_command_system *command_system, const char *const data){
	if(!type_system) return 1;
	if(!stack) return 2;
	if(!command_system) return 3;
	if(!data) return 0;

	MTEL_LIB_TYPE_size i = 0;
	while(data[i]!=MTEL_LIB_BYTE_CODE_END){
		if(data[i]==MTEL_LIB_BYTE_CODE_INT){
			i++;
			MTEL_LIB_stack_push(stack,data+i,type_system, MTEL_LIB_type_system_get_type_for_name(type_system, "int32"));
			i+=4;
		}
		else if(data[i]==MTEL_LIB_BYTE_CODE_STR){
			MTEL_LIB_TYPE_size size = 0;
			i++;
			while(data[i+size]!='\0') size++; 
			MTEL_LIB_stack_push_groups(stack, data+i, size-1, type_system,MTEL_LIB_type_system_get_type_for_name(type_system, "int8"));
			i+=++size;
		}
		else if(data[i]==MTEL_LIB_BYTE_CODE_COMMAND){
			i++;
			MTEL_LIB_command_call(command_system, *(MTEL_LIB_TYPE_command_id*)(data+i), stack, type_system);
			i++;
		}
	}

	return 0;
}
