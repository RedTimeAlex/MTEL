#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE *((MTEL_LIB_TYPE_size*)context->stack)
#define STACK_BUSY (*((MTEL_LIB_TYPE_size*)context->stack+1))
#define STACK_HEAD  ((char*)((MTEL_LIB_TYPE_size*)context->stack+2)+STACK_SIZE - STACK_BUSY - 1)


int MTEL_LIB_stack_create(MTEL_LIB_context* context,MTEL_LIB_TYPE_size size){
	if(!context) return 1;
	context->stack = (void*)malloc(2*sizeof(MTEL_LIB_TYPE_size) + size);
	if(!context->stack) return 2;
	STACK_SIZE = size;
	return 0;
}

void MTEL_LIB_stack_destuct(MTEL_LIB_context* context){
	if(!context) return;
	free(context->stack);
}


/*void MTEL_test(struct MTEL_LIB_stack* stack){
	printf("dump: \n");
	printf("size: \n");
	for(size_t i=0;i < sizeof(MTEL_LIB_TYPE_size);i++){
		printf("%X ",((char*)stack)[i]);
	}
	printf("\nbusy: \n");
	for(size_t i=0;i < sizeof(MTEL_LIB_TYPE_size);i++){
		printf("%X ",((char*)((MTEL_LIB_TYPE_size*)stack+1))[i]);
	}
	printf("\nstack: \n");
	for(size_t i=0;i < STACK_SIZE;i++){
		printf("%hhX ",((char*)((MTEL_LIB_TYPE_size*)stack+2))[i]);
	}
	printf("\n");
}*/

int MTEL_LIB_stack_push(
	MTEL_LIB_context* context,
	const void* const data,
	MTEL_LIB_TYPE_type_id type){
	if(!context) return 1;
	if(!data) return 2;
	if(!MTEL_LIB_type_system_get_size(context, type)) return 3;
	if(STACK_SIZE - STACK_BUSY < MTEL_LIB_type_system_get_size(context, type)+sizeof(MTEL_LIB_TYPE_type_id)) return 4;
	STACK_BUSY+=MTEL_LIB_type_system_get_size(context, type)-1;
	for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(context, type);i++){
		STACK_HEAD[i]=((char*)data)[i];
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = type;
	STACK_BUSY++;

	return 0;
}

int MTEL_LIB_stack_pop(
	MTEL_LIB_context* context,
	void * data){
	if(!context) return 1;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		MTEL_LIB_TYPE_size elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=MTEL_LIB_type_system_get_size(context,type)-1;
		
		elements_count--;
		if(elements_count){
			STACK_BUSY+=sizeof(MTEL_LIB_TYPE_size)-1;
			*(MTEL_LIB_TYPE_size*)STACK_HEAD = elements_count;
			STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
			*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = type;
			STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
			*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = 1;
			STACK_BUSY++;
		}
	}
	else{
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=MTEL_LIB_type_system_get_size(context,type)-1;
	}

	return 0;
}

int MTEL_LIB_stack_pop_groups(
	MTEL_LIB_context* context,
	void * data,
	MTEL_LIB_TYPE_size* elements_count){
	if(!context) return 1;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		*elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < *elements_count*MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=*elements_count*MTEL_LIB_type_system_get_size(context,type)-1;
	}
	else{
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
		STACK_BUSY++;
		return 3;
	}

	return 0;
}

int MTEL_LIB_stack_push_groups(
	MTEL_LIB_context* context,
	const void* const data, 
	MTEL_LIB_TYPE_size elements_count,
	MTEL_LIB_TYPE_type_id type){
	if(!context) return 1;
	if(!data) return 2;
	if(MTEL_LIB_type_system_get_size(context, type)) return 3;
	if(!elements_count) return 4;
	if(STACK_SIZE - STACK_BUSY < elements_count*MTEL_LIB_type_system_get_size(context, type)+2*sizeof(MTEL_LIB_TYPE_type_id)+sizeof(MTEL_LIB_TYPE_size)) return 5;
	STACK_BUSY+=elements_count*MTEL_LIB_type_system_get_size(context, type)-1;
	for(MTEL_LIB_TYPE_size i = 0; i < elements_count*MTEL_LIB_type_system_get_size(context, type);i++){
		STACK_HEAD[i]=((char*)data)[i];
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_size);
	*(MTEL_LIB_TYPE_size*)STACK_HEAD = elements_count;

	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = type;
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = 1;
	STACK_BUSY++;

	return 0;
}

int MTEL_LIB_stack_peek(
		MTEL_LIB_context* context,
		void* data){
	if(!context) return 1;
	if(!data) return 2;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		MTEL_LIB_TYPE_size elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_size);
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	}
	else{
		for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	STACK_BUSY++;

	return 0;	
}

int MTEL_LIB_stack_peek_groups(
	MTEL_LIB_context* context,
		void* data,
		MTEL_LIB_TYPE_size* elements_count
		){
	if(!context) return 1;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		*elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < *elements_count*MTEL_LIB_type_system_get_size(context, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_size);
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	}
	else{
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
		STACK_BUSY++;
		return 3;
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	STACK_BUSY++;
	return 0;
}

MTEL_LIB_TYPE_type_id MTEL_LIB_stack_get_head_type(
		MTEL_LIB_context* context
		){
	if(!context) return 0;
	MTEL_LIB_TYPE_type_id type=0;
	STACK_BUSY--;
	type=*(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	if(type==1){
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		type=*(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	}
	STACK_BUSY++;
	return type;
}
