cmake_minimum_required(VERSION 3.11)
project(so_proj2 C)


enable_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER_LOADED)
    set(CAN_USE_ASSEMBLER TRUE)
    file(GLOB_RECURSE ENGINE_ASM_FILES "*.asm")
    set(ENGINE_SOURCES ${ENGINE_SOURCES} ${ENGINE_ASM_FILES})
endif(CMAKE_ASM_NASM_COMPILER_LOADED)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g -pthread")
set(CMAKE_ASM_NASM_FLAGS "-g -DN=$ENV{THREAD_N}")




add_executable(meet_test1 meet_test1.c euron.asm euron_callbacks.c)
add_executable(meet_test2 meet_test2.c euron.asm euron_callbacks.c)
add_executable(meet_test3 meet_test3.c euron.asm euron_callbacks.c)

add_executable(meet_test1_c meet_test1.c meet.c euron_callbacks.c)
add_executable(meet_test2_c meet_test2.c meet.c euron_callbacks.c)
add_executable(meet_test3_c meet_test3.c meet.c euron_callbacks.c)


add_executable(asmimpl main.c euron.asm euron_callbacks.c)
add_executable(cimpl main.c euron_cimpl.c meet.c euron_callbacks.c)
