cmake_minimum_required(VERSION 3.16)
project(UnitConverter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(UC_ENABLE_COVERAGE "Enable gcov coverage flags" ON)

if(UC_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_library(entity_lib
    entity/unit_registry.cpp
    entity/conversion_service.cpp
)
target_include_directories(entity_lib PUBLIC "${CMAKE_SOURCE_DIR}")

add_library(data_lib
    data/json_unit_ratio_source.cpp
)
target_include_directories(data_lib PUBLIC "${CMAKE_SOURCE_DIR}")

add_library(control_lib
    control/config_loader.cpp
    control/convert_use_case.cpp
    control/register_use_case.cpp
)
target_include_directories(control_lib PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(control_lib PUBLIC entity_lib data_lib)

add_library(boundary_lib
    boundary/cli_parser.cpp
    boundary/output_formatter.cpp
    boundary/error_mapper.cpp
)
target_include_directories(boundary_lib PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(boundary_lib PUBLIC entity_lib control_lib)

add_executable(unit_converter UnitConverter.cpp)
target_link_libraries(unit_converter PRIVATE boundary_lib control_lib entity_lib data_lib)

set(GOLDEN_MASTER_WORK_DIR "${CMAKE_BINARY_DIR}/golden_master_work")
file(MAKE_DIRECTORY "${GOLDEN_MASTER_WORK_DIR}")

add_executable(UnitConverter_test
    tests/boundary/test_track_a.cpp
    tests/domain/test_track_b.cpp
    tests/integration/test_golden_master.cpp
)
target_include_directories(UnitConverter_test PRIVATE "${CMAKE_SOURCE_DIR}/tests")
target_compile_definitions(UnitConverter_test PRIVATE
    UC_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    UC_EXE_PATH="$<TARGET_FILE:unit_converter>"
    UC_TEST_WORK_DIR="${GOLDEN_MASTER_WORK_DIR}"
)
target_link_libraries(UnitConverter_test PRIVATE
    Catch2::Catch2WithMain
    boundary_lib
    control_lib
    entity_lib
    data_lib
)
add_dependencies(UnitConverter_test unit_converter)

include(CTest)
include(Catch)

catch_discover_tests(UnitConverter_test)

add_test(
    NAME GoldenMaster
    COMMAND UnitConverter_test "[golden_master]"
)
set_tests_properties(GoldenMaster PROPERTIES
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    LABELS "golden_master;regression"
)

if(UC_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage"
        COMMAND gcov -o "${CMAKE_BINARY_DIR}/CMakeFiles/entity_lib.dir/entity" "${CMAKE_SOURCE_DIR}/entity/unit_registry.cpp"
        COMMAND gcov -o "${CMAKE_BINARY_DIR}/CMakeFiles/entity_lib.dir/entity" "${CMAKE_SOURCE_DIR}/entity/conversion_service.cpp"
        COMMAND gcov -o "${CMAKE_BINARY_DIR}/CMakeFiles/boundary_lib.dir/boundary" "${CMAKE_SOURCE_DIR}/boundary/cli_parser.cpp"
        COMMAND gcov -o "${CMAKE_BINARY_DIR}/CMakeFiles/boundary_lib.dir/boundary" "${CMAKE_SOURCE_DIR}/boundary/output_formatter.cpp"
        COMMAND gcov -o "${CMAKE_BINARY_DIR}/CMakeFiles/boundary_lib.dir/boundary" "${CMAKE_SOURCE_DIR}/boundary/error_mapper.cpp"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Generate gcov artifacts for entity and boundary sources"
    )
endif()

add_custom_target(golden_master_generate
    COMMAND powershell -ExecutionPolicy Bypass -File
        "${CMAKE_SOURCE_DIR}/scripts/generate_golden_master.ps1"
        -ExePath "$<TARGET_FILE:unit_converter>" -Force
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS unit_converter
    COMMENT "Regenerate tests/golden_master_expected.txt from current CLI output"
)
