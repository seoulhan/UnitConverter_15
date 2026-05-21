#include "unit_validator.hpp"

#include <stdexcept>

namespace boundary {

void ensureKnownUnit(const entity::UnitRegistry& registry, const std::string& unitId) {
    if (!registry.metersPerUnit(unitId)) {
        throw std::invalid_argument("unknown unit: " + unitId);
    }
}

}  // namespace boundary
