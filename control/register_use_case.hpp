#pragma once

#include <optional>
#include <string>

#include "entity/domain_error.hpp"
#include "entity/unit_registry.hpp"

namespace uc::control {

class RegisterUseCase {
public:
    explicit RegisterUseCase(uc::entity::UnitRegistry registry);

    bool registerUnit(const std::string& unitId,
                      double metersPerUnit,
                      uc::entity::DomainError& error);

    uc::entity::UnitRegistry takeRegistry() { return std::move(registry_); }

    const uc::entity::UnitRegistry& registry() const { return registry_; }

private:
    uc::entity::UnitRegistry registry_;
};

}  // namespace uc::control
