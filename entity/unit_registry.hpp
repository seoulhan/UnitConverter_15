#pragma once

#include <map>
#include <optional>
#include <string>

#include "domain_error.hpp"

namespace entity {

class UnitRegistry {
public:
    static UnitRegistry defaultRegistry();

    static UnitRegistry fromUnits(const std::map<std::string, double>& units);

    [[nodiscard]] std::optional<double> metersPerUnit(const std::string& unitId) const;

    [[nodiscard]] UnitRegistry withUnit(const std::string& unitId, double metersPerOneUnit) const;

    [[nodiscard]] const std::map<std::string, double>& units() const { return units_; }

private:
    explicit UnitRegistry(std::map<std::string, double> units);

    std::map<std::string, double> units_;
};

[[nodiscard]] UnitRegistry registerUnit(const UnitRegistry& registry,
                                        const std::string& unitId,
                                        double metersPerOneUnit);

}  // namespace entity
