set(RAPIDJSON_SEARCH_PATH ${DEPENDENCIES_PATH})

find_path(RAPIDJSON_INCLUDE_PATH
    NAMES rapidjson/rapidjson.h
    PATHS ${RAPIDJSON_SEARCH_PATH}
    PATH_SUFFIXES include)
set(RAPIDJSON_FOUND "NO")
if (RAPIDJSON_INCLUDE_PATH)
    set(RAPIDJSON_FOUND "YES")
    message("EXTERNAL LIBRARY 'RAPIDJSON' FOUND")
else ()
    message("ERROR: EXTERNAL LIBRARY 'RAPIDJSON' NOT FOUND")
endif (RAPIDJSON_INCLUDE_PATH)
