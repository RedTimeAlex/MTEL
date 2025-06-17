#ifndef MTEL_LIB_STACK_H_
#define MTEL_LIB_STACK_H_
#include "mtellib/TypeSystem.h"
#include "mtellib/Types.h"

struct MTEL_LIB_stack* MTEL_LIB_stack_create(MTEL_LIB_TYPE_size size);

void MTEL_LIB_stack_destuct(struct MTEL_LIB_stack* stack);

int MTEL_LIB_stack_push(
		struct MTEL_LIB_stack* stack,
		const void* const data,
		const struct MTEL_LIB_type_system* const type_system, 
		MTEL_LIB_TYPE_type_id type);

int MTEL_LIB_stack_pop(
		struct MTEL_LIB_stack* stack,
		void* data,
		const struct MTEL_LIB_type_system* const type_system
		);

int MTEL_LIB_stack_pop_groups(
		struct MTEL_LIB_stack* stack,
		void* data,
		MTEL_LIB_TYPE_size* elements_count,
		const struct MTEL_LIB_type_system* const type_system
		);

int MTEL_LIB_stack_push_groups(
		struct MTEL_LIB_stack* stack,
		const void* const data,
		MTEL_LIB_TYPE_size elements_count,
		const struct MTEL_LIB_type_system* const type_system, 
		MTEL_LIB_TYPE_type_id type);

int MTEL_LIB_stack_peek(
		struct MTEL_LIB_stack* stack,
		void* data,
		const struct MTEL_LIB_type_system* const type_system);

int MTEL_LIB_stack_peek_groups(
		struct MTEL_LIB_stack* stack,
		void* data,
		MTEL_LIB_TYPE_size* elements_count,
		const struct MTEL_LIB_type_system* const type_system
		);

MTEL_LIB_TYPE_type_id MTEL_LIB_stack_get_head_type(
		struct MTEL_LIB_stack* stack,
		const struct MTEL_LIB_type_system* const type_system
		);
#endif
