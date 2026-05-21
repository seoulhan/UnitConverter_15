#pragma once

#include <string>

#include "data/i_unit_ratio_source.hpp"
#include "entity/unit_registry.hpp"

namespace control {

[[nodiscard]] entity::UnitRegistry loadConfigOrDefault(const std::string& path,
                                                       const data::IUnitRatioSource& source);

}  // namespace control
