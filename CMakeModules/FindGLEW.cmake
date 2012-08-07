#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

IF(MSVC)

   IF(CMAKE_CL_64)

      FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h
		$ENV{PROGRAMW6432}/GLEW/include
		DOC "The directory where GL/glew.h resides")
      FIND_LIBRARY(GLEW_LIBRARY
		NAMES glew GLEW glew32 glew32s
		PATHS
		$ENV{PROGRAMW6432}/GLEW/lib
		DOC "The GLEW library")

   ELSE(CMAKE_CL_64)
	FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
		$ENV{PROGRAMFILES}/GLEW/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES glew GLEW glew32 glew32s
		PATHS
		$ENV{PROGRAMFILES}/GLEW/lib
		DOC "The GLEW library")
   ENDIF(CMAKE_CL_64)

ELSE(MSVC)
	FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES GLEW glew
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		DOC "The GLEW library")
ENDIF(MSVC)

IF (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
        SET( GLEW_FOUND TRUE )
ENDIF (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)

IF (GLEW_FOUND)
   IF (NOT GLEW_FIND_QUIETLY)
      MESSAGE(STATUS "Found GLEW: ${GLEW_LIBRARY}")
   ENDIF (NOT GLEW_FIND_QUIETLY)
ELSE (GLEW_FOUND)
   IF (GLEW_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find GLEW")
   ENDIF (GLEW_FIND_REQUIRED)
ENDIF (GLEW_FOUND)
