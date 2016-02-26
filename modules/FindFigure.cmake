# ================================= FindFigure.cmake =======================================
# once finshed the following variables will be initialized:
#   FIGURE_INCLUDE_DIR : directory which contains all Figure files
#   FIGURE_LIBRARY : libFigure.a
# ==========================================================================================
#   Typical usage:    find_package( Figure REQUIRED )
#                     include_directories( ${FIGURE_INCLUDE_DIR} )
#                     add_executable( main my_main_file.cpp )
#                     target_link_libraries( main Figure )
# ==========================================================================================

if ( DEBUG )
  message( STATUS "Running FindFigure.cmake with DEBUG option .." )
endif()


## ----------------- find header files ------------------ ##
# get paths to figure files
set( FIGURE_PATH_SUFFIX figure )
find_path( FIGURE_HPP NAMES figure.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure header" )
find_path( FIGURECONFIG_HPP NAMES FigureConfig.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure config" )

find_path( MGL_LABEL_HPP NAMES MglLabel.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglLabel" )
find_path( MGL_PLOT_HPP NAMES MglPlot.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglPlot" )
find_path( MGL_STYLE_HPP NAMES MglStyle.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglStyle" )

set( FIGURE_PATHS ${FIGURE_HPP} 
                  ${FIGURECONFIG_HPP}
                  ${MGL_LABEL_HPP}
                  ${MGL_PLOT_HPP}
                  ${MGL_STYLE_HPP}
                  )

if ( DEBUG )
  message( STATUS "Needed files are: figure.hpp FigureConfig.hpp MglLabel.hpp MglPlot.hpp MglStyle.hpp" )
endif()

# check if the files are all in the correct place
set( PATHS_ARE_VALID true )
foreach( FILE_PATH ${FIGURE_PATHS} )
  if( NOT ${FILE_PATH} STREQUAL ${FIGURE_HPP} ) 
    set( PATHS_ARE_VALID false )

    if ( DEBUG ) # this will print which file exactly wasnt found
      message( STATUS "${FILE_PATH}" )
    endif()
  
  endif()
endforeach()

# if all filepaths are valid set FIGURE_INCLUDE_DIR, otherwise throw error/warning
if( PATHS_ARE_VALID )
  message( STATUS "Found Figure in ${FIGURE_HPP}" )
  set( FIGURE_INCLUDE_DIR ${FIGURE_HPP} )
else()
  if( Figure_REQUIRED )
    message( FATAL_ERROR "Couldn't find all necessary files for Figure, maybe try (re-)installing with administrator rights?" )
    message( "   Run cmake with the '-DDEBUG=1' option for more information on which files are missing." )
  else()
    message( STATUS "Couldn't find all necessary files for Figure, maybe try (re-)installing with administrator rights?" )
    message( "   Run cmake with the '-DDEBUG=1' option for more information on which files are missing." )
  endif()
endif()

## ----------------- find libFigure.a ------------------- ##
find_library( FIGURE_LIBRARY NAMES libFigure.a DOC "Figure library" )
if( FIGURE_LIBRARY )
  get_filename_component( FIGURE_LIBRARY_DIR ${FIGURE_LIBRARY} PATH )
  message( STATUS "Found Figure library: ${FIGURE_LIBRARY_DIR}" )
else()
  if( Figure_REQUIRED )
    message( FATAL_ERROR "Couldn't find libFigure.a, maybe try (re-)installing with administrator rights?" )
  else()
    message( STATUS "Couldn't find libFigure.a, maybe try (re-)installing with administrator rights?" )
  endif()
endif()
