#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIR
# GLEW_LIBRARY
# 

IF(MSVC)
	FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
                ../MSVC_LIBS/GLEW/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES glew GLEW glew32 glew32s
		PATHS
                ../MSVC_LIBS/GLEW/lib
		DOC "The GLEW library")
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
    SET( GLEW_FOUND TRUE)
ELSE (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    SET( GLEW_FOUND FALSE)
ENDIF (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)

MARK_AS_ADVANCED( GLEW_FOUND )
