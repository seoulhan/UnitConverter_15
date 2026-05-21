#include "data/json_unit_ratio_source.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace uc::data {

namespace {

bool extractNumber(const std::string& text, double& out) {
    try {
        out = std::stod(text);
        return true;
    } catch (...) {
        return false;
    }
}

}  // namespace

std::optional<UnitRatioSnapshot> JsonUnitRatioSource::load(
    const std::string& path) const {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;
    }

    UnitRatioSnapshot snapshot;
    std::string line;
    bool inUnits = false;
    while (std::getline(in, line)) {
        if (line.find("\"units\"") != std::string::npos) {
            inUnits = true;
            continue;
        }
        if (!inUnits) {
            continue;
        }
        const auto colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }
        auto key = line.substr(0, colon);
        const auto q1 = key.find('"');
        const auto q2 = key.rfind('"');
        if (q1 == std::string::npos || q2 == std::string::npos || q2 <= q1) {
            continue;
        }
        key = key.substr(q1 + 1, q2 - q1 - 1);

        auto valueText = line.substr(colon + 1);
        valueText.erase(
            std::remove(valueText.begin(), valueText.end(), ','),
            valueText.end());
        valueText.erase(
            std::remove(valueText.begin(), valueText.end(), ' '),
            valueText.end());

        double metersPer = 0.0;
        if (!extractNumber(valueText, metersPer)) {
            continue;
        }
        snapshot.units.emplace(key, metersPer);
    }

    if (snapshot.units.find("meter") == snapshot.units.end()) {
        return std::nullopt;
    }
    return snapshot;
}

}  // namespace uc::data
