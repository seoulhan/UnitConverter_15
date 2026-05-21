#include "config_loader.hpp"

namespace control {

entity::UnitRegistry loadConfigOrDefault(const std::string& path,
                                         const data::IUnitRatioSource& source) {
    try {
        const data::UnitConfigSnapshot snapshot = source.load(path);
        return entity::UnitRegistry::fromUnits(snapshot.units);
    } catch (...) {
        return entity::UnitRegistry::defaultRegistry();
    }
}

}  // namespace control
