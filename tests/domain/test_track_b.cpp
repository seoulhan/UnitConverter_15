#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <string>

#include "control/config_loader.hpp"
#include "data/json_unit_ratio_source.hpp"
#include "entity/conversion_service.hpp"
#include "entity/unit_constants.hpp"
#include "entity/unit_registry.hpp"

using entity::ConversionService;
using entity::UnitRegistry;
using entity::kDefaultEpsilon;
using entity::kFeetPerMeter;
using entity::kMetersPerFoot;
using entity::kYardsPerMeter;
using entity::registerUnit;

// TC-B-01 | D-INV-1
TEST_CASE("TC-B-01 convert meter to feet", "[domain][TC-B-01]") {
    ConversionService service(UnitRegistry::defaultRegistry());
    REQUIRE(service.convert("meter", 2.5, "feet") ==
            Catch::Approx(2.5 * kFeetPerMeter).epsilon(kDefaultEpsilon));
}

// TC-B-02 | D-INV-1
TEST_CASE("TC-B-02 convert meter to yard", "[domain][TC-B-02]") {
    ConversionService service(UnitRegistry::defaultRegistry());
    REQUIRE(service.convert("meter", 1.0, "yard") ==
            Catch::Approx(kYardsPerMeter).epsilon(kDefaultEpsilon));
}

// TC-B-03 | D-INV-2
TEST_CASE("TC-B-03 convert feet to meter reverse", "[domain][TC-B-03]") {
    ConversionService service(UnitRegistry::defaultRegistry());
    REQUIRE(service.convert("feet", 1.0, "meter") ==
            Catch::Approx(kMetersPerFoot).epsilon(kDefaultEpsilon));
}

// TC-B-04 | D-UC01
TEST_CASE("TC-B-04 convertAll excludes source meter", "[domain][TC-B-04]") {
    ConversionService service(UnitRegistry::defaultRegistry());
    const auto results = service.convertAll("meter", 1.0);
    REQUIRE(results.size() == 2);
    bool hasFeet = false;
    bool hasYard = false;
    bool hasMeter = false;
    for (const auto& row : results) {
        if (row.targetUnit == "feet") {
            hasFeet = true;
        } else if (row.targetUnit == "yard") {
            hasYard = true;
        } else if (row.targetUnit == "meter") {
            hasMeter = true;
        }
    }
    REQUIRE(hasFeet);
    REQUIRE(hasYard);
    REQUIRE_FALSE(hasMeter);
}

// TC-B-05 | D-INV-3
TEST_CASE("TC-B-05 register cubit then convert", "[domain][TC-B-05]") {
    const UnitRegistry registry =
        registerUnit(UnitRegistry::defaultRegistry(), "cubit", 0.4572);
    ConversionService service(registry);
    REQUIRE(service.convert("meter", 1.0, "cubit") ==
            Catch::Approx(1.0 / 0.4572).epsilon(kDefaultEpsilon));
}

// TC-B-06 | D-INV-4
TEST_CASE("TC-B-06 loadConfig valid json applies ratios", "[domain][TC-B-06]") {
    data::JsonUnitRatioSource source;
    const std::string configPath = std::string(PROJECT_SOURCE_DIR) + "/config/units.json";
    const UnitRegistry registry = control::loadConfigOrDefault(configPath, source);
    ConversionService service(registry);
    REQUIRE(service.convert("meter", 2.5, "feet") ==
            Catch::Approx(2.5 * kFeetPerMeter).epsilon(kDefaultEpsilon));
}

// TC-B-07 | D-INV-5
TEST_CASE("TC-B-07 loadConfig missing keeps defaults", "[domain][TC-B-07]") {
    data::JsonUnitRatioSource source;
    const std::string missingPath = std::string(PROJECT_SOURCE_DIR) + "/config/not_exist.json";
    const UnitRegistry registry = control::loadConfigOrDefault(missingPath, source);
    ConversionService service(registry);
    REQUIRE(service.convert("meter", 2.5, "feet") ==
            Catch::Approx(2.5 * kFeetPerMeter).epsilon(kDefaultEpsilon));
}
