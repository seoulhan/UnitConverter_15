#pragma once

#include <string>
#include <vector>

#include "entity/conversion_service.hpp"

namespace uc::boundary {

class OutputFormatter {
public:
    static std::vector<std::string> formatTable(
        const std::string& sourceValueToken,
        const std::string& sourceUnit,
        const std::vector<uc::entity::ConversionLine>& lines);
};

}  // namespace uc::boundary
