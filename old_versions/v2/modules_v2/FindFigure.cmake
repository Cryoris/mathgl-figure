# ================================= FindFigure.cmake =======================================
# once finshed the following variables will be initialized:
#   FIGURE_INCLUDE_DIR : directory which contains the Figure header ( figure.hpp )
#   FIGURE_LIBRARY     : Figure library ( libFigure.a )
#   FIGURE_LIBRARY_DIR : directory which contains the Figure library ( libFigure.a )
# ==========================================================================================
#   Typical usage:    find_package( Figure REQUIRED)
#                     include_directories( ${FIGURE_INCLUDE_DIR} )
#                     add_executable( main my_main_file.cpp )
#                     target_link_libraries( main ${FIGURE_LIBRARY} )
# ==========================================================================================

# get path to figure header
find_path(FIGURE_INCLUDE_DIR NAMES figure.hpp DOC "Figure header")
if (FIGURE_INCLUDE_DIR)
  message("-- Found Figure header in: ${FIGURE_INCLUDE_DIR}")
else ()
  message("FATAL -- could not find header of Figure ( figure.hpp ). Did you try installing it with sudo make install?")
endif()

# find libFigure.a
find_library(FIGURE_LIBRARY NAMES libFigure.a DOC "Figure library")
if (FIGURE_LIBRARY)
  get_filename_component(FIGURE_LIBRARY_DIR ${FIGURE_LIBRARY} PATH)
  message("-- Found Figure library in: ${FIGURE_LIBRARY_DIR}")
else()
  message("FATAL -- could not find library of Figure ( libFigure.a ). Did you try installing it with sudo make install?")
endif()
