#include "conversion_service.hpp"

#include <cmath>
#include <stdexcept>

namespace entity {

ConversionService::ConversionService(UnitRegistry registry) : registry_(std::move(registry)) {}

double ConversionService::convert(const std::string& fromUnit, double value,
                                  const std::string& toUnit) const {
    if (!(value > 0.0) || !std::isfinite(value)) {
        throw std::invalid_argument("value must be positive and finite");
    }

    const auto fromFactor = registry_.metersPerUnit(fromUnit);
    const auto toFactor = registry_.metersPerUnit(toUnit);
    if (!fromFactor || !toFactor) {
        throw std::invalid_argument("unknown unit");
    }

    // value_B = (value_A * R_A) / R_B  (R = meters per 1 unit)
    return (value * (*fromFactor)) / (*toFactor);
}

std::vector<ConversionResult> ConversionService::convertAll(const std::string& fromUnit,
                                                              double value) const {
    std::vector<ConversionResult> results;
    for (const auto& [unitId, factor] : registry_.units()) {
        (void)factor;
        if (unitId == fromUnit) {
            continue;
        }
        results.push_back({unitId, convert(fromUnit, value, unitId)});
    }
    return results;
}

}  // namespace entity
