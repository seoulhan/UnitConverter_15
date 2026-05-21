#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <filesystem>

#include "control/config_loader.hpp"
#include "control/convert_use_case.hpp"
#include "control/register_use_case.hpp"
#include "data/json_unit_ratio_source.hpp"
#include "entity/conversion_service.hpp"
#include "entity/unit_registry.hpp"

using Catch::Approx;

#ifndef UC_SOURCE_DIR
#define UC_SOURCE_DIR "."
#endif

TEST_CASE("TC-B-01 convert meter to feet", "[TC-B-01][domain]") {
    uc::entity::ConversionService service(uc::entity::UnitRegistry::withDefaults());
    const auto lines = service.convertAll("meter", 2.5);
    REQUIRE(lines.has_value());
    const auto feet = std::find_if(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "feet";
    });
    REQUIRE(feet != lines->end());
    REQUIRE(feet->targetValue == Approx(2.5 / 0.3048).margin(1e-9));
}

TEST_CASE("TC-B-02 convert meter to yard", "[TC-B-02][domain]") {
    uc::entity::ConversionService service(uc::entity::UnitRegistry::withDefaults());
    const auto lines = service.convertAll("meter", 2.5);
    REQUIRE(lines.has_value());
    const auto yard = std::find_if(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "yard";
    });
    REQUIRE(yard != lines->end());
    REQUIRE(yard->targetValue == Approx(2.5 / 0.9144).margin(1e-9));
}

TEST_CASE("TC-B-03 convert feet to meter reverse", "[TC-B-03][domain]") {
    uc::entity::ConversionService service(uc::entity::UnitRegistry::withDefaults());
    const auto lines = service.convertAll("feet", 1.0);
    REQUIRE(lines.has_value());
    const auto meter = std::find_if(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "meter";
    });
    REQUIRE(meter != lines->end());
    REQUIRE(meter->targetValue == Approx(0.3048).margin(1e-9));
}

TEST_CASE("TC-B-04 convertAll excludes source meter", "[TC-B-04][domain]") {
    uc::entity::ConversionService service(uc::entity::UnitRegistry::withDefaults());
    const auto lines = service.convertAll("meter", 2.5);
    REQUIRE(lines.has_value());
    REQUIRE(std::none_of(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "meter";
    }));
}

TEST_CASE("TC-B-05 register cubit then convert", "[TC-B-05][domain]") {
    uc::control::RegisterUseCase registerUseCase(uc::entity::UnitRegistry::withDefaults());
    uc::entity::DomainError error = uc::entity::DomainError::UnknownUnit;
    REQUIRE(registerUseCase.registerUnit("cubit", 0.4572, error));
    uc::entity::ConversionService service(registerUseCase.registry());
    const auto lines = service.convertAll("meter", 1.0);
    REQUIRE(lines.has_value());
    REQUIRE(std::any_of(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "cubit";
    }));
}

TEST_CASE("TC-B-06 loadConfig valid json applies ratios", "[TC-B-06][domain]") {
    const auto configPath =
        std::filesystem::path(UC_SOURCE_DIR) / "tests" / "data" / "units_custom.json";
    uc::data::JsonUnitRatioSource source;
    uc::control::ConfigLoader loader(source);
    const auto registry = loader.loadOrDefaults(configPath.string());
    uc::entity::ConversionService service(registry);
    const auto lines = service.convertAll("meter", 1.0);
    REQUIRE(lines.has_value());
    const auto feet = std::find_if(lines->begin(), lines->end(), [](const auto& line) {
        return line.targetUnit == "feet";
    });
    REQUIRE(feet != lines->end());
    REQUIRE(feet->targetValue == Approx(1.0 / 0.31).margin(1e-9));
}

TEST_CASE("TC-B-07 loadConfig missing keeps defaults", "[TC-B-07][domain]") {
    uc::data::JsonUnitRatioSource source;
    uc::control::ConfigLoader loader(source);
    const auto registry = loader.loadOrDefaults("missing_units.json");
    REQUIRE(registry.metersPerUnit("feet").has_value());
    REQUIRE(*registry.metersPerUnit("feet") == Approx(0.3048).margin(1e-9));
}

TEST_CASE("Domain register duplicate and invalid factor", "[domain][coverage]") {
    auto registry = uc::entity::UnitRegistry::withDefaults();
    uc::entity::DomainError error = uc::entity::DomainError::UnknownUnit;

    REQUIRE_FALSE(registry.registerUnit("feet", 0.31, error));
    REQUIRE(error == uc::entity::DomainError::DuplicateUnit);

    REQUIRE_FALSE(registry.registerUnit("bad", 0.0, error));
    REQUIRE(error == uc::entity::DomainError::InvalidFactor);

    REQUIRE(registry.registerUnit("cubit", 0.4572, error));
    REQUIRE(registry.metersPerUnit("cubit").has_value());
}
