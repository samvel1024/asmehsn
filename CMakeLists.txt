cmake_minimum_required(VERSION 3.11)
project(so_proj2 C)




set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g")

add_executable(euron main.c)

add_executable(meet_test meet_test.c meet.h)
target_link_libraries(meet_test pthread)
target_link_libraries(euron pthread)