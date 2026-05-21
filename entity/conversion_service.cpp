#include "entity/conversion_service.hpp"

#include <algorithm>

namespace uc::entity {

ConversionService::ConversionService(UnitRegistry registry)
    : registry_(std::move(registry)) {}

std::optional<std::vector<ConversionLine>> ConversionService::convertAll(
    const std::string& sourceUnit,
    double sourceValue) const {
    const auto sourceMetersPer = registry_.metersPerUnit(sourceUnit);
    if (!sourceMetersPer) {
        return std::nullopt;
    }

    std::vector<ConversionLine> lines;
    auto targetIds = registry_.unitIdsSorted();
    for (const auto& targetUnit : targetIds) {
        if (targetUnit == sourceUnit) {
            continue;
        }
        const auto targetMetersPer = registry_.metersPerUnit(targetUnit);
        if (!targetMetersPer) {
            continue;
        }
        const double targetValue =
            (sourceValue * (*sourceMetersPer)) / (*targetMetersPer);
        lines.push_back({targetUnit, targetValue});
    }
    return lines;
}

}  // namespace uc::entity
