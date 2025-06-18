#ifndef MTEL_LIB_COMMANDS_H_
#define MTEL_LIB_COMMANDS_H_

#include "mtellib/Types.h"

int MTEL_LIB_command_system_create(MTEL_LIB_context* context, MTEL_LIB_TYPE_size size);

void MTEL_LIB_command_system_destruct(MTEL_LIB_context* context);

int MTEL_LIB_command_system_add_command(
		MTEL_LIB_context* context,
		const char* const Name, 
		MTEL_LIB_TYPE_command_flags flags,
		void* data
		);

MTEL_LIB_TYPE_command_id MTEL_LIB_command_system_get_type_for_name(MTEL_LIB_context* context, const char* const name);

int MTEL_LIB_command_call(
		MTEL_LIB_context* context,
		MTEL_LIB_TYPE_command_id command_id);
#endif //MTEL_LIB_COMMANDS_H_
