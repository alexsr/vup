SET(ASSIMP_SEARCH_PATHS
        ${DEPENDENCIES_ROOT}
        )

FIND_PATH(ASSIMP_INCLUDE_PATH
        NAMES
        assimp/mesh.h
        PATHS
        ${ASSIMP_SEARCH_PATHS}
        PATH_SUFFIXES
        include
        DOC
        "The directory where assimp/mesh.h resides"
        )

FIND_LIBRARY(ASSIMP_LIBRARIES
        NAMES
        assimp ASSIMP libassimp.so
        PATHS
        ${ASSIMP_SEARCH_PATHS}
        PATH_SUFFIXES
        lib
        lib64
        lib/x86
        lib/x64
        DOC
        "The ASSIMP library"
        )

SET(ASSIMP_FOUND FALSE)
IF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARIES)
    SET(ASSIMP_FOUND TRUE)
    message("EXTERNAL LIBRARY 'ASSIMP' FOUND")
ELSE (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARIES)
    message("ERROR: EXTERNAL LIBRARY 'ASSIMP' NOT FOUND")
ENDIF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARIES)
