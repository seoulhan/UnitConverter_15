#include <iostream>
#include <string>

#include "boundary/cli_parser.hpp"
#include "boundary/output_formatter.hpp"
#include "boundary/unit_validator.hpp"
#include "control/config_loader.hpp"
#include "control/convert_use_case.hpp"
#include "data/json_unit_ratio_source.hpp"
#include "entity/conversion_service.hpp"

int main() {
    std::cout << "Insert value for converting (ex: meter:2.5): ";

    std::string input;
    if (!std::getline(std::cin, input)) {
        return 1;
    }

    try {
        const data::JsonUnitRatioSource source;
        const entity::UnitRegistry registry =
            control::loadConfigOrDefault("config/units.json", source);
        const boundary::ParsedInput parsed = boundary::parseConvertInput(input);
        boundary::ensureKnownUnit(registry, parsed.unitId);
        const entity::ConversionService service{registry};
        const control::ConvertUseCase useCase{service};
        const auto rows = useCase.execute(parsed.unitId, parsed.value);
        for (const auto& line : boundary::formatTable(parsed.unitId, parsed.value, rows)) {
            std::cout << line << '\n';
        }
        return 0;
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
