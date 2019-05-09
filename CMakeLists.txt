cmake_minimum_required (VERSION 2.6)
project (Compiler)

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# common.h
include_directories ("${CMAKE_HOME_DIRECTORY}/tools")

# RegExpLib module for regular expression and DFA handle
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")
include_directories ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")
link_directories ("${CMAKE_HOME_DIRECTORY}/tools/RegExp/src")

# ErrorReport module
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/FrontEnd/ErrorReport")
include_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/ErrorReport")
link_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/ErrorReport")

# Lexical Analyzer module
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Lex")
include_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Lex")
link_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Lex")

# Parser module
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Parser/src")
include_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Parser/src")
link_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd/Parser/src")

# FrontEnd Module
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/FrontEnd")
include_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd")
link_directories ("${CMAKE_HOME_DIRECTORY}/FrontEnd")

# CMinus FrontEnd
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/CMinus/FrontEnd")
include_directories ("${CMAKE_HOME_DIRECTORY}/CMinus/FrontEnd")
link_directories ("${CMAKE_HOME_DIRECTORY}/CMinus/FrontEnd")

# options
option(TestLex "compile to test Lexical Analyzer" OFF)
option(TestParser "compile to test Parser" OFF)
option(TestCMinus "compile to test CMinus" OFF)
option(NDEBUG "turn off debug" OFF)

# NDEBUG
if (NDEBUG)
  set (CMAKE_CXX_FLAGS "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
  set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

# TestLex
if (TestLex)
    add_executable (TestLex "${CMAKE_HOME_DIRECTORY}/FrontEnd/Lex/test/Main.cpp")
    target_link_libraries (TestLex Lex RegExpLib)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/FrontEnd/Lex")
    install (TARGETS TestLex
        RUNTIME DESTINATION Debug)
endif (TestLex)

# TestParser
if (TestParser)
    add_executable (TestParser "${CMAKE_HOME_DIRECTORY}/FrontEnd/Parser/src/test/Main.cpp")
    target_link_libraries (TestParser Parser)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/FrontEnd/Parser/src/")
    install (TARGETS TestParser
        RUNTIME DESTINATION Debug)
endif (TestParser)

# TestCMinus
if (TestCMinus)
    add_executable (TestCMinus "${CMAKE_HOME_DIRECTORY}/CMinus/FrontEnd/test/main.cpp")
    target_link_libraries (TestCMinus CMinusFrontEndDef FrontEnd)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}/CMinus/FrontEnd/test/")
    install (TARGETS TestCMinus
        RUNTIME DESTINATION Debug)
endif (TestCMinus)
