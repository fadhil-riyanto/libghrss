find_package( CURL )

cmake_minimum_required(VERSION 3.11)
project(ghrss)
add_library(ghrss ghrss.cc)

include_directories(/usr/include/libxml2)
target_link_libraries(ghrss curl xml2)