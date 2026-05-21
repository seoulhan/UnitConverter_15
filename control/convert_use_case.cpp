#include "control/convert_use_case.hpp"

namespace uc::control {

ConvertUseCase::ConvertUseCase(uc::entity::UnitRegistry registry)
    : service_(std::move(registry)) {}

std::optional<ConvertResult> ConvertUseCase::convert(const std::string& sourceUnit,
                                                     const std::string& sourceValueToken,
                                                     double sourceValue) {
    const auto lines = service_.convertAll(sourceUnit, sourceValue);
    if (!lines) {
        return std::nullopt;
    }
    return ConvertResult{sourceValueToken, sourceUnit, *lines};
}

}  // namespace uc::control
