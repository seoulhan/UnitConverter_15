#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "entity/converter.hpp"

using entity::convert;

// TC-B-01 | D-INV-1: 1 meter = 3.28084 feet, ε 1e-5
TEST_CASE("TC-B-01 convert meter to feet", "[domain][TC-B-01]") {
    REQUIRE(convert("meter", 2.5, "feet") == Catch::Approx(8.20210).epsilon(1e-5));
}
