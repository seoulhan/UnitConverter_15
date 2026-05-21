#pragma once

#include <optional>
#include <string>

#include "data/i_unit_ratio_source.hpp"

namespace uc::data {

class JsonUnitRatioSource : public IUnitRatioSource {
public:
    std::optional<UnitRatioSnapshot> load(const std::string& path) const override;
};

}  // namespace uc::data
