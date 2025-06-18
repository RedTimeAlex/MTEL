#ifndef MTEL_LIB_TYPES_H_
#define MTEL_LIB_TYPES_H_

typedef unsigned long MTEL_LIB_TYPE_size;
typedef unsigned char MTEL_LIB_TYPE_type_id;

typedef unsigned char MTEL_LIB_TYPE_command_id;
typedef unsigned char MTEL_LIB_TYPE_command_flags;

#define MTEL_LIB_COMMAND_FLAG_EXTR 1

struct MTEL_LIB_type_system;
typedef struct MTEL_LIB_type_system MTEL_LIB_type_system;

struct MTEL_LIB_stack;
typedef struct MTEL_LIB_stack MTEL_LIB_stack;

struct MTEL_LIB_command_system;
typedef struct MTEL_LIB_command_system MTEL_LIB_command_system;

typedef struct{
	MTEL_LIB_type_system*	 type_system;
	MTEL_LIB_stack* 	 	 stack;
	MTEL_LIB_command_system* command_system;
} MTEL_LIB_context;

#endif //MTEL_LIB_TYPES_H_
