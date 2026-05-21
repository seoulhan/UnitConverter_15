#include <iostream>
#include <string>

#include "boundary/cli_parser.hpp"
#include "boundary/error_mapper.hpp"
#include "boundary/output_formatter.hpp"
#include "control/config_loader.hpp"
#include "control/convert_use_case.hpp"
#include "control/register_use_case.hpp"
#include "data/json_unit_ratio_source.hpp"

namespace {

uc::entity::UnitRegistry gRegistry = uc::entity::UnitRegistry::withDefaults();

void printError(const uc::boundary::MappedCliError& mapped) {
    std::cerr << mapped.message << std::endl;
}

}  // namespace

int main() {
    std::string input;
    std::getline(std::cin, input);

    try {
        const auto command = uc::boundary::CliParser::parse(input);
        if (command.isRegister) {
            uc::control::RegisterUseCase registerUseCase(gRegistry);
            uc::entity::DomainError domainError = uc::entity::DomainError::UnknownUnit;
            if (!registerUseCase.registerUnit(command.registerCmd.unit,
                                              command.registerCmd.metersPerUnit,
                                              domainError)) {
                const auto mapped = uc::boundary::ErrorMapper::fromDomain(domainError);
                printError(mapped);
                return mapped.exitCode;
            }
            gRegistry = registerUseCase.takeRegistry();
            return 0;
        }

        uc::control::ConvertUseCase convertUseCase(gRegistry);
        const auto result = convertUseCase.convert(command.convert.unit,
                                                     command.convert.valueToken,
                                                     command.convert.value);
        if (!result) {
            const auto mapped =
                uc::boundary::ErrorMapper::unknownUnit(command.convert.unit);
            printError(mapped);
            return mapped.exitCode;
        }

        const auto lines = uc::boundary::OutputFormatter::formatTable(
            result->sourceValueToken, result->sourceUnit, result->lines);
        for (const auto& line : lines) {
            std::cout << line << std::endl;
        }
        return 0;
    } catch (const uc::boundary::CliException& error) {
        printError(uc::boundary::ErrorMapper::fromCli(error));
        return uc::boundary::ErrorMapper::fromCli(error).exitCode;
    }
}
