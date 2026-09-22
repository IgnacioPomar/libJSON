add_library(libJSON
    JSONArray.cpp
    JSONBase.cpp
    JSONBasicTypes.cpp
    JSONObject.cpp
    JSONParser.cpp
    JSONParseUchar.cpp
)
add_library(libJSON::libJSON ALIAS libJSON)

target_include_directories(libJSON
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(libJSON PRIVATE LIBJSON_EXPORTS)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(libJSON PUBLIC LIBJSON_DLL)
endif()

set_target_properties(libJSON PROPERTIES
    OUTPUT_NAME "libJSON"
    DEBUG_POSTFIX "d"
    POSITION_INDEPENDENT_CODE ON
)

include(GNUInstallDirs)

install(TARGETS libJSON
    EXPORT libJSONTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
