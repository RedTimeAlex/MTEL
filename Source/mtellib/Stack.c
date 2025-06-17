#include "mtellib/Stack.h"
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE *((MTEL_LIB_TYPE_size*)stack)
#define STACK_BUSY (*((MTEL_LIB_TYPE_size*)stack+1))
#define STACK_HEAD  ((char*)((MTEL_LIB_TYPE_size*)stack+2)+STACK_SIZE - STACK_BUSY - 1)


struct MTEL_LIB_stack* MTEL_LIB_stack_create(MTEL_LIB_TYPE_size size){
	void* stack = (void*)malloc(2*sizeof(MTEL_LIB_TYPE_size) + size);
	STACK_SIZE = size;
	return stack;
}

void MTEL_LIB_stack_destuct(struct MTEL_LIB_stack *stack){
	free(stack);
}


void MTEL_test(struct MTEL_LIB_stack* stack){
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
}

int MTEL_LIB_stack_push(
	struct MTEL_LIB_stack* stack, 
	const void* const data, 
	const struct MTEL_LIB_type_system* const type_system,
	MTEL_LIB_TYPE_type_id type){
	if(!stack) return 1;
	if(!data) return 2;
	if(!MTEL_LIB_type_system_get_size(type_system, type)) return 3;
	if(STACK_SIZE - STACK_BUSY < MTEL_LIB_type_system_get_size(type_system, type)+sizeof(MTEL_LIB_TYPE_type_id)) return 4;
	STACK_BUSY+=MTEL_LIB_type_system_get_size(type_system, type)-1;
	for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(type_system, type);i++){
		STACK_HEAD[i]=((char*)data)[i];
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	*(MTEL_LIB_TYPE_type_id*)STACK_HEAD = type;
	STACK_BUSY++;

	return 0;
}

int MTEL_LIB_stack_pop(struct MTEL_LIB_stack *stack, void * data, const struct MTEL_LIB_type_system *const type_system){
	if(!stack) return 1;
	if(!type_system) return 2;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		MTEL_LIB_TYPE_size elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(type_system, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=MTEL_LIB_type_system_get_size(type_system,type)-1;
		
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
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(type_system, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=MTEL_LIB_type_system_get_size(type_system,type)-1;
	}

	return 0;
}

int MTEL_LIB_stack_pop_groups(struct MTEL_LIB_stack *stack, void * data,MTEL_LIB_TYPE_size* elements_count, const struct MTEL_LIB_type_system *const type_system){
	if(!stack) return 1;
	if(!type_system) return 2;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		*elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < *elements_count*MTEL_LIB_type_system_get_size(type_system, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		STACK_BUSY-=*elements_count*MTEL_LIB_type_system_get_size(type_system,type)-1;
	}
	else{
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
		STACK_BUSY++;
		return 3;
	}

	return 0;
}

int MTEL_LIB_stack_push_groups(
	struct MTEL_LIB_stack* stack, 
	const void* const data, 
	MTEL_LIB_TYPE_size elements_count,
	const struct MTEL_LIB_type_system* const type_system,
	MTEL_LIB_TYPE_type_id type){
	if(!stack) return 1;
	if(!data) return 2;
	if(!MTEL_LIB_type_system_get_size(type_system, type)) return 3;
	if(!elements_count) return 4;
	if(STACK_SIZE - STACK_BUSY < elements_count*MTEL_LIB_type_system_get_size(type_system, type)+2*sizeof(MTEL_LIB_TYPE_type_id)+sizeof(MTEL_LIB_TYPE_size)) return 5;
	STACK_BUSY+=elements_count*MTEL_LIB_type_system_get_size(type_system, type)-1;
	for(MTEL_LIB_TYPE_size i = 0; i < elements_count*MTEL_LIB_type_system_get_size(type_system, type);i++){
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
		struct MTEL_LIB_stack* stack,
		void* data,
		const struct MTEL_LIB_type_system* const type_system){
	if(!stack) return 1;
	if(!data) return 2;
	if(!type_system) return 3;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		MTEL_LIB_TYPE_size elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(type_system, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
		
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_size);
		STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	}
	else{
		for(MTEL_LIB_TYPE_size i = 0; i < MTEL_LIB_type_system_get_size(type_system, type);i++){
			((char*)data)[i]=STACK_HEAD[i];
		}
	}
	STACK_BUSY+=sizeof(MTEL_LIB_TYPE_type_id);
	STACK_BUSY++;

	return 0;	
}

int MTEL_LIB_stack_peek_groups(
		struct MTEL_LIB_stack* stack,
		void* data,
		MTEL_LIB_TYPE_size* elements_count,
		const struct MTEL_LIB_type_system* const type_system
		){
	if(!stack) return 1;
	if(!type_system) return 2;

	STACK_BUSY--;
	MTEL_LIB_TYPE_type_id type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
	STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
	if(type==1){
		type = *(MTEL_LIB_TYPE_type_id*)STACK_HEAD;
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_type_id);
		*elements_count = *(MTEL_LIB_TYPE_size*)STACK_HEAD;		
		STACK_BUSY-=sizeof(MTEL_LIB_TYPE_size);
		
		if(data)for(MTEL_LIB_TYPE_size i = 0; i < *elements_count*MTEL_LIB_type_system_get_size(type_system, type);i++){
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
		struct MTEL_LIB_stack* stack,
		const struct MTEL_LIB_type_system* const type_system
		){
	if(!stack) return 0;
	if(!type_system) return 0;
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
