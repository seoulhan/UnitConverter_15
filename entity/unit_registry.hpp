#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "entity/domain_error.hpp"

namespace uc::entity {

class UnitRegistry {
public:
    UnitRegistry();

    static UnitRegistry withDefaults();

    static UnitRegistry fromUnits(std::map<std::string, double> units);

    const std::map<std::string, double>& units() const { return units_; }

    std::optional<double> metersPerUnit(const std::string& unitId) const;

    bool registerUnit(const std::string& unitId, double metersPerUnit, DomainError& error);

    std::vector<std::string> unitIdsSorted() const;

private:
    std::map<std::string, double> units_;
};

}  // namespace uc::entity
