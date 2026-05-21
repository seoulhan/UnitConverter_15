#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "entity/unit_registry.hpp"

namespace uc::entity {

struct ConversionLine {
    std::string targetUnit;
    double targetValue = 0.0;
};

class ConversionService {
public:
    explicit ConversionService(UnitRegistry registry);

    const UnitRegistry& registry() const { return registry_; }

    std::optional<std::vector<ConversionLine>> convertAll(const std::string& sourceUnit,
                                                          double sourceValue) const;

private:
    UnitRegistry registry_;
};

}  // namespace uc::entity
