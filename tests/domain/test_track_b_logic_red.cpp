// Track B — Domain / Logic RED (docs/RED_TESTS.md)
// Classes: ConversionService, UnitRegistry, JsonUnitRatioSource, loadConfigOrDefault

#include <catch2/catch_test_macros.hpp>

// TC-B-RED-01 | D-INV-1
// Scenario: convert("meter", 2.5, "feet") — 1 meter = 3.28084 feet, ε 1e-5
TEST_CASE("test_convert_meter_to_feet_within_1e5", "[domain][red][TC-B-RED-01]") {
    FAIL("RED");
}

// TC-B-RED-02 | D-INV-1
// Scenario: convert("meter", 1.0, "yard") — 1 meter = 1.09361 yard
TEST_CASE("test_convert_meter_to_yard_within_1e5", "[domain][red][TC-B-RED-02]") {
    FAIL("RED");
}

// TC-B-RED-03 | D-INV-2
// Scenario: convert("feet", 1.0, "meter") — 1 foot = 0.3048 meter
TEST_CASE("test_convert_feet_to_meter_reverse", "[domain][red][TC-B-RED-03]") {
    FAIL("RED");
}

// TC-B-RED-04 | D-UC01
// Scenario: convertAll("meter", 1.0) — feet, yard only (no meter target)
TEST_CASE("test_convert_all_excludes_source_meter", "[domain][red][TC-B-RED-04]") {
    FAIL("RED");
}

// TC-B-RED-05 | D-INV-3
// Scenario: registerUnit("cubit", 0.4572) then convert
TEST_CASE("test_register_cubit_then_convert", "[domain][red][TC-B-RED-05]") {
    FAIL("RED");
}

// TC-B-RED-06 | D-INV-4
// Scenario: loadConfig("config/units.json") applies ratios
TEST_CASE("test_load_config_json_applies_ratios", "[domain][red][TC-B-RED-06]") {
    FAIL("RED");
}

// TC-B-RED-07 | D-INV-5
// Scenario: loadConfig(invalid_path) keeps default 3.28084 / 1.09361
TEST_CASE("test_load_config_missing_keeps_defaults", "[domain][red][TC-B-RED-07]") {
    FAIL("RED");
}
