#ifndef MTEL_LIB_EXECUTE_H_
#define MTEL_LIB_EXECUTE_H_

#include "mtellib/Types.h"

#define MTEL_LIB_BYTE_CODE_END 0
#define MTEL_LIB_BYTE_CODE_INT 1
#define MTEL_LIB_BYTE_CODE_STR 2
#define MTEL_LIB_BYTE_CODE_COMMAND 3

int MTEL_LIB_execute(MTEL_LIB_context* context, const char* const data);

#endif //MTEL_LIB_EXECUTE_H_
