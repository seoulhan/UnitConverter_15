#pragma once

#include "i_unit_ratio_source.hpp"

namespace data {

class JsonUnitRatioSource : public IUnitRatioSource {
public:
    [[nodiscard]] UnitConfigSnapshot load(const std::string& path) const override;
};

[[nodiscard]] UnitConfigSnapshot loadYamlUnitConfig(const std::string& path);

}  // namespace data
