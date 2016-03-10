# to use this Figure library :
#                 find_package( FIGURE REQUIRED )
#                 include_directories( ${FIGURE_INCLUDE_DIR} )
#                 add_executable( exec_name exec_source.cpp )
#                 target_link_libraries( exec_name Figure )

cmake_minimum_required( VERSION 2.8 )
project( FigureClass )

# definitions needed to build libFigure.a properly
add_definitions( -Wall -std=gnu++11 -fext-numeric-literals )

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../modules/ )

# set path to src directory
set( SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src/ )

# set all header files in variable HEADER_FILES
set( HEADER_FILES ${SRC}/figure.hpp
                  ${SRC}/FigureConfig.hpp
                  ${SRC}/MglLabel.hpp
                  ${SRC}/MglPlot.hpp
                  ${SRC}/MglStyle.hpp )

# find and include Eigen and MathGL
find_package( Eigen3 REQUIRED )
find_package( MathGL2 2.0.0 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR}  ${MATHGL2_INCLUDE_DIRS} )

# build library
add_library( Figure ${SRC}/figure.cpp )
target_link_libraries( Figure ${MATHGL2_LIBRARIES} )

# install library and header files
install( TARGETS Figure 
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib )
install( FILES ${HEADER_FILES} 
         DESTINATION include/figure )
