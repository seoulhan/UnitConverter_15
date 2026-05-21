#include "json_unit_ratio_source.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace {

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("config file not found: " + path);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

data::UnitConfigSnapshot parseUnitsObject(const std::string& content) {
    data::UnitConfigSnapshot snapshot;
    snapshot.version = 1;
    snapshot.baseUnit = "meter";

    const std::regex pairRegex(R"rx("([a-z][a-z0-9_]*)"\s*:\s*([0-9]+(?:\.[0-9]+)?))rx");
    const auto begin = std::sregex_iterator(content.begin(), content.end(), pairRegex);
    const auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        const std::string unitId = (*it)[1].str();
        const double factor = std::stod((*it)[2].str());
        if (factor <= 0.0) {
            throw std::runtime_error("unit factor must be positive");
        }
        snapshot.units[unitId] = factor;
    }

    if (snapshot.units.find("meter") == snapshot.units.end()) {
        throw std::runtime_error("meter unit is required");
    }
    if (snapshot.units.empty()) {
        throw std::runtime_error("units section is empty");
    }
    return snapshot;
}

}  // namespace

namespace data {

UnitConfigSnapshot JsonUnitRatioSource::load(const std::string& path) const {
    const std::string content = readFile(path);
    return parseUnitsObject(content);
}

UnitConfigSnapshot loadYamlUnitConfig(const std::string& path) {
    const std::string content = readFile(path);
    return parseUnitsObject(content);
}

}  // namespace data
