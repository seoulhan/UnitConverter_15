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
  entity/converter.cpp
)
target_include_directories(entity_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(unit_converter UnitConverter.cpp)

add_executable(unit_converter_tests tests/test_converter.cpp)
target_link_libraries(unit_converter_tests PRIVATE entity_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(unit_converter_tests)
