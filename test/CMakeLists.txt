cmake_minimum_required(VERSION 3.11)
project(ghrss)
add_executable(test_get get.cc)
target_link_libraries(test_get ${CMAKE_CURRENT_SOURCE_DIR}/../build/libghrss.a -lbacktrace)