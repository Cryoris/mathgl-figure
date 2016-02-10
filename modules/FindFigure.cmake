# ================================= FindFigure.cmake =======================================
# once finshed the following variables will be initialized:
#   FIGURE_INCLUDE_DIR : directory which contains all Figure files
# ==========================================================================================
#   Typical usage:    find_package( Figure REQUIRED)
#                     include_directories( ${FIGURE_INCLUDE_DIR} )
#                     add_executable( main my_main_file.cpp )
# ==========================================================================================

if ( DEBUG )
  message( STATUS "Running FindFigure.cmake with DEBUG option .." )
endif()

# get paths to figure files
set( FIGURE_PATH_SUFFIX figure )
find_path( FIGURE_HPP NAMES figure.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure header" )
find_path( FIGURE_CPP NAMES figure.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure source" )
find_path( FIGURE NAMES Figure PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure include" )
find_path( FIGURECONFIG_HPP NAMES FigureConfig.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "Figure config" )

find_path( MGL_LABEL_HPP NAMES MglLabel.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglLabel" )
find_path( MGL_PLOT_HPP NAMES MglPlot.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglPlot" )
find_path( MGL_STYLE_HPP NAMES MglStyle.hpp PATH_SUFFIXES ${FIGURE_PATH_SUFFIX} DOC "MglStyle" )

set( FIGURE_PATHS ${FIGURE_HPP} 
                  ${FIGURE_CPP}
                  ${FIGURE}
                  ${FIGURECONFIG_HPP}
                  ${MGL_LABEL_HPP}
                  ${MGL_PLOT_HPP}
                  ${MGL_STYLE_HPP}
                  )

if ( DEBUG )
  message( STATUS "Needed files are: figure.hpp figure.cpp Figure FigureConfig.hpp MglLabel.hpp MglPlot.hpp MglStyle.hpp" )
endif()

# check if the files are all in the correct place
set( PATHS_ARE_VALID true )
foreach( FILE_PATH ${FIGURE_PATHS} )
  if( NOT ${FILE_PATH} STREQUAL ${FIGURE} ) 
    set( PATHS_ARE_VALID false )

    if ( DEBUG ) # this will print which file exactly wasnt found
      message( STATUS "${FILE_PATH}" )
    endif()
  
  endif()
endforeach()

# if all filepaths are valid set FIGURE_INCLUDE_DIR, otherwise throw error/warning
if( PATHS_ARE_VALID )
  message( STATUS "Found Figure in ${FIGURE}" )
  set( FIGURE_INCLUDE_DIR ${FIGURE} )
else()
  if( Figure_REQUIRED )
    message(FATAL "Couldn't find all necessary files for Figure, maybe try (re-)installing with administrator rights?")
    message("   Run cmake with the '-DDEBUG=1' option for more information on which files are missing.")
  else()
    message(STATUS "Couldn't find all necessary files for Figure, maybe try (re-)installing with administrator rights?")
    message("   Run cmake with the '-DDEBUG=1' option for more information on which files are missing.")
  endif()
endif()
