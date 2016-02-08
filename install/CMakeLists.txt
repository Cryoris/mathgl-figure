# to use this Figure library :
#                 find_package( FIGURE REQUIRED )
#                 include_directories( ${FIGURE_INCLUDE_DIR} )
#                 add_executable(exec_name exec_source.cpp)

cmake_minimum_required( VERSION 2.8 )
project( FigureClass )

set( SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src/ )

set( CORE_FILES ${SRC}/figure.hpp
                ${SRC}/figure.cpp
                ${SRC}/FigureConfig.hpp
                ${SRC}/Figure
                )

set( UTILITY_FILES ${SRC}/MglLabel.hpp
                   ${SRC}/MglPlot.hpp
                   ${SRC}/MglStyle.hpp
                   )

set( TARGET_FILES ${CORE_FILES} ${UTILITY_FILES} )

install( FILES ${TARGET_FILES} DESTINATION include/figure )
