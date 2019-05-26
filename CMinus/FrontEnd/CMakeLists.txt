# NDEBUG
if (NDEBUG)
  set (CMAKE_CXX_FLAGS "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
  set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

add_library (CMinusFrontEndDef FrontEndImplement.cpp MCode.cpp)

