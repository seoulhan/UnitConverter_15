#include "DecayingUnit.hpp"

#include <stdexcept>

DecayingUnit::DecayingUnit() : registry_(uc::entity::UnitRegistry::withDefaults()) {}

void DecayingUnit::registerUnit(const std::string& unitId, double metersPerUnit) {
    uc::entity::DomainError error = uc::entity::DomainError::UnknownUnit;
    if (!registry_.registerUnit(unitId, metersPerUnit, error)) {
        if (error == uc::entity::DomainError::InvalidFactor) {
            throw std::invalid_argument("meters per unit must be positive");
        }
        throw std::invalid_argument("duplicate unit: " + unitId);
    }
}

double DecayingUnit::convert(const std::string& fromUnit, double value,
                             const std::string& toUnit) const {
    const auto fromFactor = registry_.metersPerUnit(fromUnit);
    const auto toFactor = registry_.metersPerUnit(toUnit);
    if (!fromFactor || !toFactor) {
        throw std::invalid_argument("unknown unit");
    }
    return (value * (*fromFactor)) / (*toFactor);
}

std::map<std::string, double> DecayingUnit::convertAll(const std::string& fromUnit,
                                                       double value) const {
    std::map<std::string, double> results;
    for (const auto& [unitId, factor] : registry_.units()) {
        (void)factor;
        results[unitId] = convert(fromUnit, value, unitId);
    }
    return results;
}
