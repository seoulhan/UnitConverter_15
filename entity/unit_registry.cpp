#include "entity/unit_registry.hpp"

namespace uc::entity {

namespace {

constexpr double kMetersPerMeter = 1.0;
constexpr double kMetersPerFoot = 0.3048;
constexpr double kMetersPerYard = 0.9144;

}  // namespace

UnitRegistry::UnitRegistry() = default;

UnitRegistry UnitRegistry::withDefaults() {
    return fromUnits({
        {"feet", kMetersPerFoot},
        {"meter", kMetersPerMeter},
        {"yard", kMetersPerYard},
    });
}

UnitRegistry UnitRegistry::fromUnits(std::map<std::string, double> units) {
    UnitRegistry registry;
    registry.units_ = std::move(units);
    return registry;
}

std::optional<double> UnitRegistry::metersPerUnit(const std::string& unitId) const {
    const auto it = units_.find(unitId);
    if (it == units_.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool UnitRegistry::registerUnit(const std::string& unitId,
                                double metersPerUnit,
                                DomainError& error) {
    if (metersPerUnit <= 0.0) {
        error = DomainError::InvalidFactor;
        return false;
    }
    if (units_.count(unitId) != 0) {
        error = DomainError::DuplicateUnit;
        return false;
    }
    units_.emplace(unitId, metersPerUnit);
    return true;
}

std::vector<std::string> UnitRegistry::unitIdsSorted() const {
    std::vector<std::string> ids;
    ids.reserve(units_.size());
    for (const auto& entry : units_) {
        ids.push_back(entry.first);
    }
    return ids;
}

}  // namespace uc::entity
