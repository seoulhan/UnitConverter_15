#pragma once

#include <map>
#include <optional>
#include <string>

namespace uc::data {

struct UnitRatioSnapshot {
    std::map<std::string, double> units;
};

class IUnitRatioSource {
public:
    virtual ~IUnitRatioSource() = default;
    virtual std::optional<UnitRatioSnapshot> load(const std::string& path) const = 0;
};

}  // namespace uc::data
