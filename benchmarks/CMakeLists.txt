cmake_minimum_required(VERSION 3.21)
project(libJSON_benchmarks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(LIBJSON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(libJSON_for_bench STATIC
    ${LIBJSON_ROOT}/src/JSONArray.cpp
    ${LIBJSON_ROOT}/src/JSONBase.cpp
    ${LIBJSON_ROOT}/src/JSONBasicTypes.cpp
    ${LIBJSON_ROOT}/src/JSONObject.cpp
    ${LIBJSON_ROOT}/src/JSONParser.cpp
    ${LIBJSON_ROOT}/src/JSONParseUchar.cpp
)
target_include_directories(libJSON_for_bench PUBLIC ${LIBJSON_ROOT}/include)
target_compile_features(libJSON_for_bench PUBLIC cxx_std_17)

find_package(nlohmann_json REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(simdjson REQUIRED)
find_package(glaze REQUIRED)

add_executable(json_benchmark
    src/main.cpp
    src/candle_data.cpp
    src/bench_libjson.cpp
    src/bench_nlohmann.cpp
    src/bench_rapidjson.cpp
    src/bench_simdjson.cpp
    src/bench_glaze.cpp
)

# glaze needs C++20; the other translation units are C++17-compatible and build fine at C++20 too.
target_compile_features(json_benchmark PRIVATE cxx_std_20)

target_link_libraries(json_benchmark PRIVATE
    libJSON_for_bench
    nlohmann_json::nlohmann_json
    rapidjson
    simdjson::simdjson
    glaze::glaze
)
