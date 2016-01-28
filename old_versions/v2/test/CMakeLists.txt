cmake_minimum_required(VERSION 2.8)
project(Testing)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)

add_definitions(-std=gnu++11)
find_package(MathGL2 2.0.0 REQUIRED)

include_directories(${MATHGL2_INCLUDE_DIRS})

set(FIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../ncse/NumCSE/MathGL/FigureClass)
#set(FIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
add_library(Figure ${FIG_DIR}/figure.cpp)
target_link_libraries(Figure ${MATHGL2_LIBRARIES})

file(GLOB 2dTestFiles "2d-*.cpp")
foreach(testFile ${2dTestFiles})
  # get name for Tests */2d-x.cpp -> T2d-x for x = 1..9
  string(REGEX MATCH "(2d-[1-9])" testExeName ${testFile})
  set(testExeName "T${testExeName}")
  add_executable(${testExeName} 2d-1.cpp)
  target_link_libraries(${testExeName} Figure)

  set(2dTests ${2dTests} ${testExeName})
endforeach()

file(GLOB 3dTestFiles "3d-*.cpp")
foreach(testFile ${3dTestFiles})
  # get name for Tests */3d-x.cpp -> T3d-x for x = 1..9
  string(REGEX MATCH "(3d-[1-9])" testExeName ${testFile})
  set(testExeName "T${testExeName}")
  add_executable(${testExeName} 3d-1.cpp)
  target_link_libraries(${testExeName} Figure)

  set(3dTests ${3dTests} ${testExeName})
endforeach()


message(STATUS "2dTests: ${2dTests}")
message(STATUS "3dTests: ${3dTests}")

enable_testing()
foreach(test ${2dTests} ${3dTests})
  add_test(${test} ${test})
endforeach()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${2dTests} ${3dTests})
