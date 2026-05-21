#pragma once

#include <string>

#include "entity/unit_registry.hpp"

namespace boundary {

void ensureKnownUnit(const entity::UnitRegistry& registry, const std::string& unitId);

}  // namespace boundary
