#include "mtellib/Execute.h"
#include "mtellib/Commands.h"
#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"

#include <stdio.h>

int MTEL_LIB_execute(MTEL_LIB_context* context, const char *const data){
	if(!context) return 1;
	if(!data) return 0;

	MTEL_LIB_TYPE_size i = 0;
	while(data[i]!=MTEL_LIB_BYTE_CODE_END){
		if(data[i]==MTEL_LIB_BYTE_CODE_INT){
			i++;
			MTEL_LIB_stack_push(context,data+i, MTEL_LIB_type_system_get_type_for_name(context, "int32"));
			i+=4;
		}
		else if(data[i]==MTEL_LIB_BYTE_CODE_STR){
			MTEL_LIB_TYPE_size size = 0;
			i++;
			while(data[i+size]!='\0') size++; 
			MTEL_LIB_stack_push_groups(context, data+i, size-1, MTEL_LIB_type_system_get_type_for_name(context, "int8"));
			printf("exec str\n");
			i+=++size;
		}
		else if(data[i]==MTEL_LIB_BYTE_CODE_COMMAND){
			i++;
			MTEL_LIB_command_call(context, *(MTEL_LIB_TYPE_command_id*)(data+i));
			i++;
		}
	}

	return 0;
}
