cmake_minimum_required(VERSION 2.8)
  

PROJECT(base_sql_parser CXX)

add_compile_options(-std=c++11)

aux_source_directory(. base_sql_parser_SRC)

execute_process(COMMAND bash ../mm)

set(LIB_SRC ../../../lib)

add_library(base_sql_parser STATIC ${base_sql_parser_SRC})

set(LIBRARY_OUTPUT_PATH ${LIB_SRC})
