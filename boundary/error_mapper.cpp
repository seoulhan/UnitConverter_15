#include "boundary/error_mapper.hpp"

namespace uc::boundary {

MappedCliError ErrorMapper::fromCli(const CliException& error) {
    MappedCliError mapped;
    mapped.exitCode = 2;
    mapped.message = error.what();
    if (error.kind() == CliErrorKind::UnknownUnit) {
        mapped.exitCode = 3;
    }
    return mapped;
}

MappedCliError ErrorMapper::unknownUnit(const std::string& unitId) {
    return {3, "Unknown unit: " + unitId};
}

MappedCliError ErrorMapper::fromDomain(uc::entity::DomainError error) {
    switch (error) {
        case uc::entity::DomainError::UnknownUnit:
            return {3, "Unknown unit"};
        case uc::entity::DomainError::DuplicateUnit:
            return {2, "Duplicate unit registration"};
        case uc::entity::DomainError::InvalidFactor:
            return {2, "Invalid meters per unit factor"};
    }
    return {2, "Domain error"};
}

}  // namespace uc::boundary
