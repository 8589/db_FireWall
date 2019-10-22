cmake_minimum_required(VERSION 3.1)


PROJECT(CJsonObjecti C CXX)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  #设置c++的编译选项
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项

add_compile_options(-std=c++11)

#include_directories(../../include)

aux_source_directory(. CJsonObject_SRC)

set(LIB_SRC ../../../lib)

add_library(CJsonObject STATIC ${CJsonObject_SRC})
#target_link_libraries(CJsonObject cJSON)

set(LIBRARY_OUTPUT_PATH ${LIB_SRC})
