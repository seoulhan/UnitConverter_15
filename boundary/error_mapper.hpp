#pragma once

#include "boundary/cli_parser.hpp"
#include "entity/domain_error.hpp"

namespace uc::boundary {

struct MappedCliError {
    int exitCode = 2;
    std::string message;
};

class ErrorMapper {
public:
    static MappedCliError fromCli(const CliException& error);
    static MappedCliError unknownUnit(const std::string& unitId);
    static MappedCliError fromDomain(uc::entity::DomainError error);
};

}  // namespace uc::boundary
