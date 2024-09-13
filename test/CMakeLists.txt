cmake_minimum_required(VERSION 3.11)
project(ghrss)
add_executable(test_get get.cc)
target_link_libraries(test_get ${CMAKE_CURRENT_SOURCE_DIR}/../build/libghrss.a curl fmt xml2 leveldb)


add_executable(test_node node.cc)
include_directories(/usr/include/libxml2)
target_link_libraries(test_node xml2)

add_executable(rocksdbtest rocksdbtest.cc)
target_link_libraries(rocksdbtest rocksdb)

add_executable(leveldbtest leveldbtest.cc)
target_link_libraries(leveldbtest leveldb)

add_executable(writeasint writeasint.cc)
target_link_libraries(writeasint leveldb)