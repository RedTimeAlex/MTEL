#ifndef MTEL_LIB_TYPE_SYSTEM_H_
#define MTEL_LIB_TYPE_SYSTEM_H_

#include "mtellib/Types.h"

struct MTEL_LIB_type_system* MTEL_LIB_type_system_create(MTEL_LIB_TYPE_size size);

void MTEL_LIB_type_system_destruct(struct MTEL_LIB_type_system* type_system);

int MTEL_LIB_type_system_add_type(
		struct MTEL_LIB_type_system* type_system, 
		const char* const typeName, 
		MTEL_LIB_TYPE_size size);

MTEL_LIB_TYPE_type_id MTEL_LIB_type_system_get_type_for_name(const struct MTEL_LIB_type_system* const type_system, const char* const name);

MTEL_LIB_TYPE_size MTEL_LIB_type_system_get_size(const struct MTEL_LIB_type_system* const type_system, MTEL_LIB_TYPE_type_id type);

#endif //MTEL_LIB_TYPE_SYSTEM_H_
