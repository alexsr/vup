#
# Find GLM include path.
#
# GLM_FOUND
# GLM_INCLUDE_PATH
#

SET(GLM_SEARCH_PATHS
        ${DEPENDENCIES_ROOT}/include
        )

FIND_PATH(GLM_INCLUDE_PATH
        NAMES
        glm/glm.hpp
        PATHS
        ${GLM_SEARCH_PATHS}
        DOC
        "The directory where glm/glm.hpp resides"
        )

SET(GLM_FOUND "NO")
IF (GLM_INCLUDE_PATH)
    SET(GLM_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLM' FOUND")
ELSE ()
    message("ERROR: EXTERNAL LIBRARY 'GLM' NOT FOUND")
ENDIF (GLM_INCLUDE_PATH)
