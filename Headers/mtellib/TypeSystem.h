#ifndef MTEL_LIB_TYPE_SYSTEM_H_
#define MTEL_LIB_TYPE_SYSTEM_H_

#include "mtellib/Types.h"

int MTEL_LIB_type_system_create(MTEL_LIB_context* context,MTEL_LIB_TYPE_size size);

void MTEL_LIB_type_system_destruct(MTEL_LIB_context* context);

int MTEL_LIB_type_system_add_type(
		MTEL_LIB_context* context,
		const char* const typeName, 
		MTEL_LIB_TYPE_size size);

MTEL_LIB_TYPE_type_id MTEL_LIB_type_system_get_type_for_name(
		MTEL_LIB_context* context,
		const char* const name);

MTEL_LIB_TYPE_size MTEL_LIB_type_system_get_size(
		MTEL_LIB_context* context,
		MTEL_LIB_TYPE_type_id type);

#endif //MTEL_LIB_TYPE_SYSTEM_H_
