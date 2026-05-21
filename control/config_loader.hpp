#pragma once

#include <string>

#include "data/i_unit_ratio_source.hpp"
#include "entity/unit_registry.hpp"

namespace uc::control {

class ConfigLoader {
public:
    explicit ConfigLoader(const uc::data::IUnitRatioSource& source);

    uc::entity::UnitRegistry loadOrDefaults(const std::string& path) const;

private:
    const uc::data::IUnitRatioSource& source_;
};

}  // namespace uc::control
