SET(OPENGL_SEARCH_PATHS
        ${DEPENDENCIES_ROOT}
        )

IF (WIN32)
    FIND_PATH(GLEW_INCLUDE_PATH
            NAMES
            GL/glew.h
            PATHS
            ${OPENGL_SEARCH_PATHS}
            PATH_SUFFIXES
            include
            DOC
            "The directory where GL/glew.h resides"
            )

    FIND_LIBRARY(GLEW_LIBRARY
            NAMES
            glew32s.lib
            PATHS
            ${OPENGL_SEARCH_PATHS}
            PATH_SUFFIXES
            lib
            DOC
            "The directory where glew32s.lib resides"
            )


ELSEIF (APPLE OR UNIX)
    FIND_PATH(GLEW_INCLUDE_PATH
            NAMES
            GL/glew.h
            PATHS
            ${OPENGL_SEARCH_PATHS}
            PATH_SUFFIXES
            include
            DOC
            "The directory where GL/glew.h resides"
            )

    FIND_LIBRARY(GLEW_LIBRARY
            NAMES
            libGLEW.a
            PATHS
            ${OPENGL_SEARCH_PATHS}
            PATH_SUFFIXES
            lib
            DOC
            "The directory where libGLEW.a resides"
            )

ELSE ()
    FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h)
    FIND_LIBRARY(GLEW_LIBRARY
            NAMES GLEW glew32 glew glew32s glew32sd PATH_SUFFIXES lib64)
ENDIF ()


SET(GLEW_FOUND "NO")
IF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
    SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
    SET(GLEW_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLEW' FOUND")
ELSE ()
    message("ERROR: EXTERNAL LIBRARY 'GLEW' NOT FOUND")
ENDIF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)