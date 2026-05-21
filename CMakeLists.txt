cmake_minimum_required(VERSION 3.16)
project(UnitConverter VERSION 1.0 LANGUAGES CXX)

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

add_library(entity_lib
  entity/unit_registry.cpp
  entity/conversion_service.cpp
)
target_include_directories(entity_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(data_lib
  data/json_unit_ratio_source.cpp
)
target_include_directories(data_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(data_lib PUBLIC entity_lib)

add_library(boundary_lib
  boundary/cli_parser.cpp
  boundary/output_formatter.cpp
  boundary/unit_validator.cpp
)
target_include_directories(boundary_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(boundary_lib PUBLIC entity_lib)

add_library(control_lib
  control/convert_use_case.cpp
  control/config_loader.cpp
)
target_include_directories(control_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(control_lib PUBLIC entity_lib data_lib boundary_lib)

add_executable(unit_converter UnitConverter.cpp)

# RED phase: skeleton tests only (FAIL("RED")), Catch2 only
set(TEST_SOURCES
  tests/boundary/test_track_a_ui_red.cpp
  tests/domain/test_track_b_logic_red.cpp
)

add_executable(unit_converter_tests ${TEST_SOURCES})
target_link_libraries(unit_converter_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(unit_converter_tests)
