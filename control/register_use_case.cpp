#include "control/register_use_case.hpp"

namespace uc::control {

RegisterUseCase::RegisterUseCase(uc::entity::UnitRegistry registry)
    : registry_(std::move(registry)) {}

bool RegisterUseCase::registerUnit(const std::string& unitId,
                                   double metersPerUnit,
                                   uc::entity::DomainError& error) {
    return registry_.registerUnit(unitId, metersPerUnit, error);
}

}  // namespace uc::control
