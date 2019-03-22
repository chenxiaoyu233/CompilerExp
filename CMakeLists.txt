cmake_minimum_required (VERSION 2.6)
project (Compiler)

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# common.h
include_directories ("${CMAKE_HOME_DIRECTORY}/tools")

# RegExpLib module for regular expression and DFA handle
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")
include_directories ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")
link_directories ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")

# Lexical Analyzer module
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/Lex")
include_directories ("${CMAKE_HOME_DIRECTORY}/Lex")
link_directories ("${CMAKE_HOME_DIRECTORY}/Lex")


# options
option(TestLex "compile to test Lexical Analyzer" OFF)
option(NDEBUG "turn off debug" OFF)

# NDEBUG
if (NDEBUG)
  set (CMAKE_CXX_FLAGS "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
  set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

# TestLex
if (TestLex)
  add_executable (TestLex "${CMAKE_HOME_DIRECTORY}/Lex/test/Main.cpp")
  target_link_libraries (TestLex Lex RegExpLib)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/Lex")
  install (TARGETS TestLex
	RUNTIME DESTINATION Debug)
endif (TestLex)

