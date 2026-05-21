#pragma once

#include <stdexcept>
#include <string>

namespace uc::boundary {

enum class CliErrorKind {
    InvalidFormat,
    InvalidNumber,
    NonPositiveValue,
    UnknownUnit,
    InvalidRegisterFormat,
    DuplicateUnit,
    InvalidFactor,
};

class CliException : public std::runtime_error {
public:
    CliException(CliErrorKind kind, const std::string& message);

    CliErrorKind kind() const { return kind_; }

private:
    CliErrorKind kind_;
};

struct ConvertCommand {
    std::string unit;
    std::string valueToken;
    double value = 0.0;
};

struct RegisterCommand {
    std::string unit;
    double metersPerUnit = 0.0;
};

struct ParsedCommand {
    bool isRegister = false;
    ConvertCommand convert{};
    RegisterCommand registerCmd{};
};

class CliParser {
public:
    static ParsedCommand parse(const std::string& line);
};

}  // namespace uc::boundary
