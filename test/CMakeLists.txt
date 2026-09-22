find_package(GTest REQUIRED)

add_executable(libJSONTester
    src/001_CreateJSON.cpp
    src/002_parseJSON.cpp
    src/003_parseTypesJSON.cpp
    src/004_Iterate.cpp
)

target_include_directories(libJSONTester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(libJSONTester PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
target_link_libraries(libJSONTester PRIVATE libJSON::libJSON GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(libJSONTester)
