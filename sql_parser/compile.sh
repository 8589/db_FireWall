flex lex.l
cp /usr/include/FlexLexer.h  .
g++ test_sql_parser.cpp sql_parser.cpp lex.yy.cc FlexLexer.h -o test

