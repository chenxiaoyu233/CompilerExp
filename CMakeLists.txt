cmake_minimum_required (VERSION 2.6)
project (Compiler)

# common.h
include_directories ("${CMAKE_HOME_DIRECTORY}/tools")

# RegExpLib module for regular expression and DFA handle
include_directories ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")

# Lexical Analyzer module
include_directories ("${CMAKE_HOME_DIRECTORY}/Lex")
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/Lex")


# options
option(TestLex "compile to test Lexical Analyzer" OFF)

# compile
if (TestLex)
  add_executable (TestLex "${CMAKE_HOME_DIRECTORY}/Lex/test/Main.cpp")
  target_link_libraries (TestLex RegExpLib Lex)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/Lex")
  install (TARGETS TestLex
	RUNTIME DESTINATION Debug)
endif (TestLex)

