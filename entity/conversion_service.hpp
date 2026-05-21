#pragma once

#include <string>
#include <vector>

#include "unit_registry.hpp"

namespace entity {

struct ConversionResult {
    std::string targetUnit;
    double value;
};

class ConversionService {
public:
    explicit ConversionService(UnitRegistry registry);

    [[nodiscard]] double convert(const std::string& fromUnit, double value,
                                 const std::string& toUnit) const;

    [[nodiscard]] std::vector<ConversionResult> convertAll(const std::string& fromUnit,
                                                           double value) const;

    [[nodiscard]] const UnitRegistry& registry() const { return registry_; }

private:
    UnitRegistry registry_;
};

}  // namespace entity
