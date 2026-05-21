#pragma once

#include <map>
#include <string>

namespace data {

struct UnitConfigSnapshot {
    int version = 1;
    std::string baseUnit = "meter";
    std::map<std::string, double> units;
};

class IUnitRatioSource {
public:
    virtual ~IUnitRatioSource() = default;
    [[nodiscard]] virtual UnitConfigSnapshot load(const std::string& path) const = 0;
};

}  // namespace data
