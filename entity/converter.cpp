#include "converter.hpp"

#include "unit_constants.hpp"

namespace entity {

double convert(const std::string& fromUnit, double value, const std::string& toUnit) {
    if (fromUnit == "meter" && toUnit == "feet") {
        return value * kFeetPerMeter;
    }
    if (fromUnit == "meter" && toUnit == "yard") {
        return value * kYardsPerMeter;
    }
    return 0.0;
}

}  // namespace entity
