#pragma once

#include <optional>
#include <string>
#include <vector>

#include "entity/conversion_service.hpp"

namespace uc::control {

struct ConvertResult {
    std::string sourceValueToken;
    std::string sourceUnit;
    std::vector<uc::entity::ConversionLine> lines;
};

class ConvertUseCase {
public:
    explicit ConvertUseCase(uc::entity::UnitRegistry registry);

    std::optional<ConvertResult> convert(const std::string& sourceUnit,
                                           const std::string& sourceValueToken,
                                           double sourceValue);

    const uc::entity::UnitRegistry& registry() const { return service_.registry(); }

private:
    uc::entity::ConversionService service_;
};

}  // namespace uc::control
