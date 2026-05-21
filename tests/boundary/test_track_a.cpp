#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "boundary/cli_parser.hpp"
#include "boundary/error_mapper.hpp"
#include "boundary/output_formatter.hpp"
#include "control/convert_use_case.hpp"
#include "entity/domain_error.hpp"
#include "entity/unit_registry.hpp"

using Catch::Approx;

TEST_CASE("TC-A-01 happy path meter colon 2p5", "[TC-A-01][boundary]") {
    const auto command = uc::boundary::CliParser::parse("meter:2.5");
    REQUIRE_FALSE(command.isRegister);
    REQUIRE(command.convert.unit == "meter");
    REQUIRE(command.convert.value == 2.5);
}

TEST_CASE("TC-A-02 missing colon throws", "[TC-A-02][boundary]") {
    REQUIRE_THROWS_AS(uc::boundary::CliParser::parse("meter2.5"),
                      uc::boundary::CliException);
}

TEST_CASE("TC-A-03 negative value throws", "[TC-A-03][boundary]") {
    REQUIRE_THROWS_AS(uc::boundary::CliParser::parse("meter:-1.5"),
                      uc::boundary::CliException);
}

TEST_CASE("TC-A-04 unknown unit throws", "[TC-A-04][boundary]") {
    uc::control::ConvertUseCase useCase(uc::entity::UnitRegistry::withDefaults());
    REQUIRE_FALSE(useCase.convert("furlong", "1", 1.0).has_value());
}

TEST_CASE("TC-A-05 invalid number throws", "[TC-A-05][boundary]") {
    REQUIRE_THROWS_AS(uc::boundary::CliParser::parse("meter:abc"),
                      uc::boundary::CliException);
}

TEST_CASE("TC-A-06 output preserves source unit and value", "[TC-A-06][boundary]") {
    uc::control::ConvertUseCase useCase(uc::entity::UnitRegistry::withDefaults());
    const auto result = useCase.convert("meter", "2.5", 2.5);
    REQUIRE(result.has_value());
    const auto lines = uc::boundary::OutputFormatter::formatTable(
        result->sourceValueToken, result->sourceUnit, result->lines);
    REQUIRE(lines.size() == 2);
    REQUIRE(lines[0].rfind("2.5 meter =", 0) == 0);
    REQUIRE(lines[1].rfind("2.5 meter =", 0) == 0);
}

TEST_CASE("TC-A-07 zero value throws", "[TC-A-07][boundary]") {
    REQUIRE_THROWS_AS(uc::boundary::CliParser::parse("meter:0.0"),
                      uc::boundary::CliException);
}

TEST_CASE("Boundary register command and error mapper", "[boundary][coverage]") {
    const auto command =
        uc::boundary::CliParser::parse("register:cubit=0.4572");
    REQUIRE(command.isRegister);
    REQUIRE(command.registerCmd.unit == "cubit");
    REQUIRE(command.registerCmd.metersPerUnit == Approx(0.4572));

    REQUIRE_THROWS_AS(uc::boundary::CliParser::parse("register:cubit"),
                      uc::boundary::CliException);

    uc::boundary::CliException formatError(
        uc::boundary::CliErrorKind::InvalidFormat,
        "Invalid format. Use unit:value (ex: meter:2.5)");
    const auto mapped = uc::boundary::ErrorMapper::fromCli(formatError);
    REQUIRE(mapped.exitCode == 2);

    const auto unknown = uc::boundary::ErrorMapper::unknownUnit("furlong");
    REQUIRE(unknown.exitCode == 3);
    REQUIRE(unknown.message == "Unknown unit: furlong");

    const auto domainMapped =
        uc::boundary::ErrorMapper::fromDomain(uc::entity::DomainError::DuplicateUnit);
    REQUIRE(domainMapped.exitCode == 2);
}
