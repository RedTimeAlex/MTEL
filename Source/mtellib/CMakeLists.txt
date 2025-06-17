project(mtellib VERSION 0.0.1)

add_library(${PROJECT_NAME} SHARED TypeSystem.c Stack.c Commands.c Execute.c)

target_include_directories(${PROJECT_NAME} PRIVATE  "${CMAKE_SOURCE_DIR}/Headers")
