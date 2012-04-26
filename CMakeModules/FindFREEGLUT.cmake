# Try to find the FREEGLUT library
#
# FREEGLUT_INCLUDE_DIR
# FREEGLUT_LIBRARY
# FREEGLUT_FOUND

IF(WIN32)

   FIND_PATH( FREEGLUT_INCLUDE_DIR GL/freeglut.h
      ../dependences/FREEGLUT/include
      DOC "The directory where GL/freeflut.h resides")
   FIND_LIBRARY( FREEGLUT_LIBRARY
      NAMES freeglut_static freeglut glut
      PATHS
      ../dependences/FREEGLUT/lib
      DOC "The FREEGLUT library")
   
ELSE(WIN32)

   FIND_PATH(
      FREEGLUT_INCLUDE_DIR GL/freeglut.h
      ${CMAKE_INCLUDE_PATH}
      $ENV{include}
      ${OPENGL_INCLUDE_DIR}
      /usr/include
      /usr/local/include)

   FIND_LIBRARY(
      FREEGLUT_LIBRARY
      NAMES freeglut_static freeglut glut
      PATH
      ${CMAKE_LIBRARY_PATH}
      $ENV{lib}
      /usr/lib
      /usr/local/lib)


ENDIF(WIN32)

IF (FREEGLUT_INCLUDE_DIR AND FREEGLUT_LIBRARY)
   SET(FREEGLUT_FOUND TRUE)
ENDIF (FREEGLUT_INCLUDE_DIR AND FREEGLUT_LIBRARY)

IF (FREEGLUT_FOUND)
   IF (NOT FREEGLUT_FIND_QUIETLY)
      MESSAGE(STATUS "Found FREEGLUT: ${FREEGLUT_LIBRARY}")
   ENDIF (NOT FREEGLUT_FIND_QUIETLY)
ELSE (FREEGLUT_FOUND)
   IF (FREEGLUT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find FREEGLUT")
   ENDIF (FREEGLUT_FIND_REQUIRED)
ENDIF (FREEGLUT_FOUND)
