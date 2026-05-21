cmake_minimum_required(VERSION 3.16)
project(UnitConverter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_converter UnitConverter.cpp)

add_executable(unit_converter_tests
    tests/integration/test_golden_master.cpp
)
target_include_directories(unit_converter_tests PRIVATE "${CMAKE_SOURCE_DIR}/tests")
target_compile_definitions(unit_converter_tests PRIVATE
    UC_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    UC_EXE_PATH="$<TARGET_FILE:unit_converter>"
)
target_link_libraries(unit_converter_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(unit_converter_tests)

add_custom_target(golden_master_generate
    COMMAND "${CMAKE_SOURCE_DIR}/scripts/generate_golden_master.sh"
        "$<TARGET_FILE:unit_converter>"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS unit_converter
    COMMENT "Regenerate tests/golden_master_expected.txt from current CLI output"
)
