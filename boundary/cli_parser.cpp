#include "boundary/cli_parser.hpp"

#include <cmath>

namespace uc::boundary {

CliException::CliException(CliErrorKind kind, const std::string& message)
    : std::runtime_error(message), kind_(kind) {}

namespace {

void trimInPlace(std::string& text) {
    while (!text.empty() && text.front() == ' ') {
        text.erase(text.begin());
    }
    while (!text.empty() && text.back() == ' ') {
        text.pop_back();
    }
}

double parsePositiveNumber(const std::string& token) {
    try {
        const double value = std::stod(token);
        if (!std::isfinite(value) || value <= 0.0) {
            throw CliException(
                CliErrorKind::NonPositiveValue,
                "Value must be positive: " + token);
        }
        return value;
    } catch (const CliException&) {
        throw;
    } catch (...) {
        throw CliException(CliErrorKind::InvalidNumber,
                           "Invalid number: " + token);
    }
}

}  // namespace

ParsedCommand CliParser::parse(const std::string& line) {
    if (line.rfind("register:", 0) == 0) {
        const std::string payload = line.substr(std::string("register:").size());
        const auto eq = payload.find('=');
        if (eq == std::string::npos) {
            throw CliException(
                CliErrorKind::InvalidRegisterFormat,
                "Invalid register format. Use register:unit=meters_per_unit "
                "(ex: register:cubit=0.4572)");
        }
        RegisterCommand cmd;
        cmd.unit = payload.substr(0, eq);
        trimInPlace(cmd.unit);
        const std::string factorToken = payload.substr(eq + 1);
        cmd.metersPerUnit = parsePositiveNumber(factorToken);
        ParsedCommand parsed;
        parsed.isRegister = true;
        parsed.registerCmd = cmd;
        return parsed;
    }

    const auto colon = line.find(':');
    if (colon == std::string::npos) {
        throw CliException(
            CliErrorKind::InvalidFormat,
            "Invalid format. Use unit:value (ex: meter:2.5)");
    }

    ConvertCommand cmd;
    cmd.unit = line.substr(0, colon);
    trimInPlace(cmd.unit);
    cmd.valueToken = line.substr(colon + 1);
    trimInPlace(cmd.valueToken);
    cmd.value = parsePositiveNumber(cmd.valueToken);

    ParsedCommand parsed;
    parsed.convert = cmd;
    return parsed;
}

}  // namespace uc::boundary
