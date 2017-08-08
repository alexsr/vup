#
# Find GLM include path.
#
# GLM_FOUND
# GLM_INCLUDE_PATH
#

SET(RAPIDJSON_SEARCH_PATH
        ${DEPENDENCIES_ROOT}
        )

FIND_PATH(RAPIDJSON_INCLUDE_PATH
        NAMES
        rapidjson/rapidjson.h
        PATHS
        ${RAPIDJSON_SEARCH_PATH}
        PATH_SUFFIXES
        include
        DOC
        "The directory where rapidjson/rapidjson.h resides"
        )

SET(RAPIDJSON_FOUND "NO")
IF (RAPIDJSON_INCLUDE_PATH)
    SET(RAPIDJSON_FOUND "YES")
    message("EXTERNAL LIBRARY 'RAPIDJSON' FOUND")
ELSE ()
    message("ERROR: EXTERNAL LIBRARY 'RAPIDJSON' NOT FOUND")
ENDIF (RAPIDJSON_INCLUDE_PATH)
