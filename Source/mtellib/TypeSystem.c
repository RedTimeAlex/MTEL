#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"
#include <stdlib.h>

#define TYPE_SYSTEM_SIZE (*(MTEL_LIB_TYPE_size*)context->type_system)
#define TYPE_SYSTEM_TYPE_COUNT (*((MTEL_LIB_TYPE_size*)context->type_system+1))
#define TYPE_SYSTEM_TYPES_ARRAY ((struct type* )((MTEL_LIB_TYPE_size*)context->type_system+2))

struct type{
	char* name;
	MTEL_LIB_TYPE_size size;
};

int MTEL_LIB_type_system_create(MTEL_LIB_context* context,MTEL_LIB_TYPE_size size){
	if(!context) return 1;
	context->type_system = malloc(2*sizeof(MTEL_LIB_TYPE_size)+size*sizeof(struct type));
	if(!context->type_system) return 2;
	TYPE_SYSTEM_SIZE = size;
	TYPE_SYSTEM_TYPE_COUNT=0;
	return 0;
}

void MTEL_LIB_type_system_destruct(MTEL_LIB_context* context){
	while(TYPE_SYSTEM_TYPE_COUNT){
		free(TYPE_SYSTEM_TYPES_ARRAY[TYPE_SYSTEM_TYPE_COUNT].name);
		TYPE_SYSTEM_TYPE_COUNT--;
	}

	free(context->type_system);
}

int MTEL_LIB_type_system_add_type(
		MTEL_LIB_context* context,
		const char* const typeName, 
		MTEL_LIB_TYPE_size size){
	if(!context) return 1;
	if(!typeName)	 return 2;
	if(!size) 		 return 3;
	if(TYPE_SYSTEM_TYPE_COUNT==TYPE_SYSTEM_SIZE) return 4;

	MTEL_LIB_TYPE_size nameSize = 0;
	while(typeName[nameSize]!='\0'){
		nameSize++;
	}
	char* name = malloc(nameSize+1);
	if(!name) return 5;
	for(MTEL_LIB_TYPE_size i = 0; i <= nameSize;i++){
		name[i]=typeName[i];
	}
	name[nameSize+1]='\0';

	TYPE_SYSTEM_TYPES_ARRAY[TYPE_SYSTEM_TYPE_COUNT]= (struct type){name,size};
	TYPE_SYSTEM_TYPE_COUNT++;

	return 0;
}

MTEL_LIB_TYPE_size MTEL_LIB_type_system_get_size(MTEL_LIB_context* context, MTEL_LIB_TYPE_type_id type){
	if(type < 2) return 0;
	type -= 2; 
	if(!context) return 0;
	if(type >= TYPE_SYSTEM_TYPE_COUNT && type == 1) return 0;

	return TYPE_SYSTEM_TYPES_ARRAY[type].size;
}

MTEL_LIB_TYPE_type_id MTEL_LIB_type_system_get_type_for_name(MTEL_LIB_context* context, const char* const name){
	if(!context) return 0;
	if(!name) return 0;

	for(MTEL_LIB_TYPE_size i = 0; i < TYPE_SYSTEM_TYPE_COUNT;i++){
		MTEL_LIB_TYPE_size cmp = 0;	
		while(TYPE_SYSTEM_TYPES_ARRAY[i].name[cmp]!='\0' && name[cmp]!='\0'){
			if(TYPE_SYSTEM_TYPES_ARRAY[i].name[cmp]!=name[cmp]){
				cmp = 0; break;
			}
			cmp++;
		}

		if(cmp) return i + 2;
	}
	return 0;
}
