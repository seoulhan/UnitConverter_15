#pragma once

#include <string>
#include <vector>

#include "entity/conversion_service.hpp"

namespace boundary {

[[nodiscard]] std::string formatTableLine(double sourceValue, const std::string& sourceUnit,
                                         double targetValue, const std::string& targetUnit);

[[nodiscard]] std::vector<std::string> formatTable(const std::string& sourceUnit,
                                                   double sourceValue,
                                                   const std::vector<entity::ConversionResult>& rows);

}  // namespace boundary
