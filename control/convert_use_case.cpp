#include "convert_use_case.hpp"

namespace control {

ConvertUseCase::ConvertUseCase(entity::ConversionService service) : service_(std::move(service)) {}

std::vector<entity::ConversionResult> ConvertUseCase::execute(const std::string& fromUnit,
                                                               double value) const {
    return service_.convertAll(fromUnit, value);
}

}  // namespace control
