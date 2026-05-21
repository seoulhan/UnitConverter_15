#include "control/config_loader.hpp"

namespace uc::control {

ConfigLoader::ConfigLoader(const uc::data::IUnitRatioSource& source)
    : source_(source) {}

uc::entity::UnitRegistry ConfigLoader::loadOrDefaults(const std::string& path) const {
    const auto snapshot = source_.load(path);
    if (!snapshot) {
        return uc::entity::UnitRegistry::withDefaults();
    }

    return uc::entity::UnitRegistry::fromUnits(snapshot->units);
}

}  // namespace uc::control
