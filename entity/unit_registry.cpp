#include "unit_registry.hpp"

#include <stdexcept>

#include "unit_constants.hpp"

namespace entity {

UnitRegistry::UnitRegistry(std::map<std::string, double> units) : units_(std::move(units)) {}

UnitRegistry UnitRegistry::defaultRegistry() {
    return UnitRegistry({{"meter", kMetersPerMeter},
                         {"feet", kMetersPerFoot},
                         {"yard", kMetersPerYard}});
}

UnitRegistry UnitRegistry::fromUnits(const std::map<std::string, double>& units) {
    if (units.find("meter") == units.end()) {
        throw std::invalid_argument("meter unit is required");
    }
    for (const auto& [id, factor] : units) {
        if (factor <= 0.0) {
            throw std::invalid_argument("unit factor must be positive: " + id);
        }
    }
    return UnitRegistry(units);
}

std::optional<double> UnitRegistry::metersPerUnit(const std::string& unitId) const {
    const auto it = units_.find(unitId);
    if (it == units_.end()) {
        return std::nullopt;
    }
    return it->second;
}

UnitRegistry UnitRegistry::withUnit(const std::string& unitId, double metersPerOneUnit) const {
    if (metersPerOneUnit <= 0.0) {
        throw std::invalid_argument("meters per unit must be positive");
    }
    auto copy = units_;
    if (copy.count(unitId) != 0) {
        throw std::invalid_argument("duplicate unit: " + unitId);
    }
    copy[unitId] = metersPerOneUnit;
    return UnitRegistry(std::move(copy));
}

UnitRegistry registerUnit(const UnitRegistry& registry, const std::string& unitId,
                          double metersPerOneUnit) {
    return registry.withUnit(unitId, metersPerOneUnit);
}

}  // namespace entity
