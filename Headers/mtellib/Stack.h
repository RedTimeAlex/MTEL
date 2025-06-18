#ifndef MTEL_LIB_STACK_H_
#define MTEL_LIB_STACK_H_
#include "mtellib/Types.h"

int MTEL_LIB_stack_create(MTEL_LIB_context* context,MTEL_LIB_TYPE_size size);

void MTEL_LIB_stack_destuct(MTEL_LIB_context* context);

int MTEL_LIB_stack_push(
		MTEL_LIB_context* context,
		const void* const data,
		MTEL_LIB_TYPE_type_id type);

int MTEL_LIB_stack_pop(
		MTEL_LIB_context* context,
		void* data
		);

int MTEL_LIB_stack_pop_groups(
		MTEL_LIB_context* context,
		void* data,
		MTEL_LIB_TYPE_size* elements_count
		);

int MTEL_LIB_stack_push_groups(
		MTEL_LIB_context* context,
		const void* const data,
		MTEL_LIB_TYPE_size elements_count,
		MTEL_LIB_TYPE_type_id type);

int MTEL_LIB_stack_peek(
		MTEL_LIB_context* context,
		void* data);

int MTEL_LIB_stack_peek_groups(
		MTEL_LIB_context* context,
		void* data,
		MTEL_LIB_TYPE_size* elements_count
		);

MTEL_LIB_TYPE_type_id MTEL_LIB_stack_get_head_type(
		MTEL_LIB_context* context
		);
#endif
