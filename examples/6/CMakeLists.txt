cmake_minimum_required( VERSION 2.8 )
project( Examples/6 )

set( SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../src )
set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules )

add_definitions( -std=gnu++11 )

find_package( Eigen3 REQUIRED )
find_package( MathGL2 2.0.0 REQUIRED )
include_directories( ${SRC} ${EIGEN_INCLUDE_DIR} ${MATHGL2_INCLUDE_DIRS} )

add_executable( main multiple.cpp )
target_link_libraries( main ${MATHGL2_LIBRARIES} )
