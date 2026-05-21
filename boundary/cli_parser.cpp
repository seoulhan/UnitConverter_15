#include "cli_parser.hpp"

#include <cctype>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace {

std::string trim(const std::string& text) {
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) {
        ++start;
    }
    std::size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }
    return text.substr(start, end - start);
}

bool isValidUnitId(const std::string& unitId) {
    if (unitId.empty() || unitId.size() > 32) {
        return false;
    }
    if (!std::islower(static_cast<unsigned char>(unitId.front()))) {
        return false;
    }
    for (std::size_t i = 1; i < unitId.size(); ++i) {
        const char c = unitId[i];
        if (!std::islower(static_cast<unsigned char>(c)) && !std::isdigit(static_cast<unsigned char>(c)) &&
            c != '_') {
            return false;
        }
    }
    return true;
}

double parsePositiveNumber(const std::string& raw) {
    if (raw.empty()) {
        throw std::invalid_argument("empty number");
    }
    std::size_t dotCount = 0;
    for (char c : raw) {
        if (c == '.') {
            ++dotCount;
            if (dotCount > 1) {
                throw std::invalid_argument("malformed decimal");
            }
            continue;
        }
        if (c == '-' || c == '+') {
            throw std::invalid_argument("signed number not allowed here");
        }
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("invalid number token");
        }
    }

    std::istringstream stream(raw);
    double value = 0.0;
    stream >> value;
    if (!stream || !stream.eof()) {
        throw std::invalid_argument("invalid number");
    }
    if (!(value > 0.0) || !std::isfinite(value)) {
        throw std::invalid_argument("value must be positive");
    }
    return value;
}

}  // namespace

namespace boundary {

ParsedInput parseConvertInput(const std::string& line) {
    const std::string trimmed = trim(line);
    const std::size_t colon = trimmed.find(':');
    if (colon == std::string::npos) {
        throw std::invalid_argument("missing colon in unit:value format");
    }

    ParsedInput parsed;
    parsed.unitId = trim(trimmed.substr(0, colon));
    const std::string valueText = trim(trimmed.substr(colon + 1));

    if (!isValidUnitId(parsed.unitId)) {
        throw std::invalid_argument("invalid unit id");
    }

    parsed.value = parsePositiveNumber(valueText);
    return parsed;
}

ParsedRegister parseRegisterInput(const std::string& line) {
    const std::string trimmed = trim(line);
    if (trimmed.rfind("register:", 0) != 0) {
        throw std::invalid_argument("register prefix required");
    }
    const std::string body = trim(trimmed.substr(std::string("register:").size()));
    const std::size_t eq = body.find('=');
    if (eq == std::string::npos) {
        throw std::invalid_argument("register format unit=factor required");
    }

    ParsedRegister parsed;
    parsed.unitId = trim(body.substr(0, eq));
    const std::string factorText = trim(body.substr(eq + 1));
    if (!isValidUnitId(parsed.unitId)) {
        throw std::invalid_argument("invalid unit id");
    }
    parsed.metersPerUnit = parsePositiveNumber(factorText);
    return parsed;
}

}  // namespace boundary
