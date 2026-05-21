#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "boundary/cli_parser.hpp"
#include "boundary/output_formatter.hpp"
#include "boundary/unit_validator.hpp"
#include "control/convert_use_case.hpp"
#include "entity/conversion_service.hpp"
#include "entity/unit_registry.hpp"

namespace {

std::vector<std::string> convertLines(const std::string& input) {
    const entity::UnitRegistry registry = entity::UnitRegistry::defaultRegistry();
    const boundary::ParsedInput parsed = boundary::parseConvertInput(input);
    boundary::ensureKnownUnit(registry, parsed.unitId);
    const entity::ConversionService service{registry};
    const control::ConvertUseCase useCase{service};
    const auto rows = useCase.execute(parsed.unitId, parsed.value);
    return boundary::formatTable(parsed.unitId, parsed.value, rows);
}

}  // namespace

// TC-A-01 | INV-A-01
TEST_CASE("TC-A-01 happy path meter colon 2p5", "[boundary][TC-A-01]") {
    const auto lines = convertLines("meter:2.5");
    REQUIRE(lines.size() == 2);
    const std::string joined = lines[0] + lines[1];
    REQUIRE(joined.find("2.5 meter =") != std::string::npos);
    REQUIRE(joined.find("feet") != std::string::npos);
    REQUIRE(joined.find("yard") != std::string::npos);
}

// TC-A-02 | INV-A-02
TEST_CASE("TC-A-02 missing colon throws", "[boundary][TC-A-02]") {
    REQUIRE_THROWS_AS(boundary::parseConvertInput("meter2.5"), std::invalid_argument);
}

// TC-A-03 | INV-A-03
TEST_CASE("TC-A-03 negative value throws", "[boundary][TC-A-03]") {
    REQUIRE_THROWS_AS(boundary::parseConvertInput("meter:-1.0"), std::invalid_argument);
}

// TC-A-04 | INV-A-04
TEST_CASE("TC-A-04 unknown unit throws", "[boundary][TC-A-04]") {
    const entity::UnitRegistry registry = entity::UnitRegistry::defaultRegistry();
    const boundary::ParsedInput parsed = boundary::parseConvertInput("parsec:1.0");
    REQUIRE_THROWS_AS(boundary::ensureKnownUnit(registry, parsed.unitId), std::invalid_argument);
}

// TC-A-05 | INV-A-05
TEST_CASE("TC-A-05 invalid number throws", "[boundary][TC-A-05]") {
    REQUIRE_THROWS_AS(boundary::parseConvertInput("meter:abc"), std::invalid_argument);
}

// TC-A-06 | INV-A-06
TEST_CASE("TC-A-06 output preserves source unit and value", "[boundary][TC-A-06]") {
    const auto lines = convertLines("meter:2.5");
    REQUIRE_FALSE(lines.empty());
    for (const auto& line : lines) {
        REQUIRE(line.find("2.5 meter =") == 0);
    }
}

// TC-A-07 | INV-A-03
TEST_CASE("TC-A-07 zero value throws", "[boundary][TC-A-07]") {
    REQUIRE_THROWS_AS(boundary::parseConvertInput("meter:0"), std::invalid_argument);
}
