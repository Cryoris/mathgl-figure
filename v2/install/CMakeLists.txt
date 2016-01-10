# to use this Figure library :
# include the directory containing the files figure.cpp and figure.hpp and modify your CMakeLists.txt as follows:
#                 add_library(Figure figure.cpp)
#                 add_executable(exec_name exec_source.cpp)
#                 target_link_libraries(exec_name Figure)

cmake_minimum_required(VERSION 2.8)
project(FigureClass)

add_definitions(-Wall)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(MathGL2 2.0.0 REQUIRED)
include_directories(${MATHGL2_INCLUDE_DIRS})

add_library(Figure STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../figure.cpp)
target_link_libraries(Figure ${MATHGL2_LIBRARIES})

install(TARGETS Figure 
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../figure.hpp DESTINATION include)
