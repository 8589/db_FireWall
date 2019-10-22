cmake_minimum_required(VERSION 3.1)
  

PROJECT(sql_parser CXX)

add_compile_options(-std=c++11)

aux_source_directory(. sql_parser_SRC)

execute_process(COMMAND bash ../mm)

set(LIB_SRC ../../../lib)

add_library(sql_parser STATIC ${sql_parser_SRC})

set(LIBRARY_OUTPUT_PATH ${LIB_SRC})
