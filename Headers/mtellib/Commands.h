#ifndef MTEL_LIB_COMMANDS_H_
#define MTEL_LIB_COMMANDS_H_

#include "mtellib/Types.h"
#include "mtellib/Stack.h"

struct MTEL_LIB_command_system* MTEL_LIB_command_system_create(MTEL_LIB_TYPE_size size);

void MTEL_LIB_command_system_destruct(struct MTEL_LIB_command_system* command_system);

int MTEL_LIB_command_system_add_command(
		struct MTEL_LIB_command_system* command_system, 
		const char* const Name, 
		MTEL_LIB_TYPE_command_flags flags,
		void* data
		);

MTEL_LIB_TYPE_command_id MTEL_LIB_command_system_get_type_for_name(const struct MTEL_LIB_command_system* const command_system, const char* const name);

int MTEL_LIB_command_call(
	const struct MTEL_LIB_command_system* const commands_system, 
	MTEL_LIB_TYPE_command_id command_id, 
	struct MTEL_LIB_stack* stack,
 	struct MTEL_LIB_type_system* type_system);
#endif //MTEL_LIB_COMMANDS_H_
