// Track A — UI / Boundary RED (docs/RED_TESTS.md)
// Classes: CliParser, OutputFormatter, unit_validator (Boundary)

#include <catch2/catch_test_macros.hpp>

// TC-A-RED-01 | INV-A-01
// Given: "meter:2.5"; 1 meter = 3.28084 feet, 1 meter = 1.09361 yard
// When:  parseConvertInput → convert → formatTable
// Then:  "2.5 meter = 8.202100 feet", exit 0
TEST_CASE("test_parse_convert_meter_colon_2p5_returns_conversion_lines",
          "[boundary][red][TC-A-RED-01]") {
    FAIL("RED");
}

// TC-A-RED-02 | INV-A-02
// Given: "meter2.5" (no colon)
// When:  parseConvertInput
// Then:  std::invalid_argument
TEST_CASE("test_parse_missing_colon_throws_invalid_argument",
          "[boundary][red][TC-A-RED-02]") {
    FAIL("RED");
}

// TC-A-RED-03 | INV-A-03
// Given: "meter:-1.0"
// When:  parseConvertInput
// Then:  std::invalid_argument
TEST_CASE("test_parse_negative_meter_throws_invalid_argument",
          "[boundary][red][TC-A-RED-03]") {
    FAIL("RED");
}

// TC-A-RED-04 | INV-A-04
// Given: "parsec:1.0", unit not in UnitRegistry
// When:  parseConvertInput → ensureKnownUnit
// Then:  std::invalid_argument
TEST_CASE("test_parse_unknown_parsec_throws_invalid_argument",
          "[boundary][red][TC-A-RED-04]") {
    FAIL("RED");
}

// TC-A-RED-05 | INV-A-05
// Given: source meter, 2.5
// When:  formatTable
// Then:  "2.5 meter = 8.202100 feet" (LHS preserved)
TEST_CASE("test_format_table_preserves_source_unit_and_value",
          "[boundary][red][TC-A-RED-05]") {
    FAIL("RED");
}

// TC-A-RED-06 | INV-A-06
// Given: "meter:2.5", --format=json
// When:  JsonFormatter / formatJson
// Then:  README §6.2 schema (source + conversions[])
TEST_CASE("test_format_json_meter_2p5_matches_schema",
          "[boundary][red][TC-A-RED-06]") {
    FAIL("RED");
}

// TC-A-RED-07 | INV-A-03
// Given: "meter:0"
// When:  parseConvertInput
// Then:  std::invalid_argument
TEST_CASE("test_parse_zero_meter_throws_invalid_argument",
          "[boundary][red][TC-A-RED-07]") {
    FAIL("RED");
}
