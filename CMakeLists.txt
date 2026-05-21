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

set(GOLDEN_MASTER_WORK_DIR "${CMAKE_BINARY_DIR}/golden_master_work")
file(MAKE_DIRECTORY "${GOLDEN_MASTER_WORK_DIR}")

add_executable(UnitConverter_test
    tests/integration/test_golden_master.cpp
)
target_include_directories(UnitConverter_test PRIVATE "${CMAKE_SOURCE_DIR}/tests")
target_compile_definitions(UnitConverter_test PRIVATE
    UC_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    UC_EXE_PATH="$<TARGET_FILE:unit_converter>"
    UC_TEST_WORK_DIR="${GOLDEN_MASTER_WORK_DIR}"
)
target_link_libraries(UnitConverter_test PRIVATE Catch2::Catch2WithMain)
add_dependencies(UnitConverter_test unit_converter)

include(CTest)
add_test(
    NAME GoldenMaster
    COMMAND UnitConverter_test "[golden_master]"
)
set_tests_properties(GoldenMaster PROPERTIES
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    LABELS "golden_master;regression"
)

add_custom_target(golden_master_generate
    COMMAND "${CMAKE_SOURCE_DIR}/scripts/generate_golden_master.sh"
        "$<TARGET_FILE:unit_converter>"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS unit_converter
    COMMENT "Regenerate tests/golden_master_expected.txt from current CLI output"
)
