#pragma once

#include <string>

namespace boundary {

struct ParsedInput {
    std::string unitId;
    double value;
};

struct ParsedRegister {
    std::string unitId;
    double metersPerUnit;
};

[[nodiscard]] ParsedInput parseConvertInput(const std::string& line);
[[nodiscard]] ParsedRegister parseRegisterInput(const std::string& line);

}  // namespace boundary
