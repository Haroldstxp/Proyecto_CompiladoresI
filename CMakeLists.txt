cmake_minimum_required(VERSION 3.20)
project (Proyecto_Lexer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Proyecto_Lexer
main.cpp
LexerP.hpp
LexerP.cpp
ParserP.hpp
ParserP.cpp
)