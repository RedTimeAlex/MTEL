project(mtel LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} PRIVATE  "${CMAKE_SOURCE_DIR}/Headers")
target_link_libraries(${PROJECT_NAME} mtellib)
