#include "output_formatter.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace boundary {

namespace {

double roundOneDecimal(double value) {
    return std::round(value * 10.0) / 10.0;
}

std::string formatNumber(double value) {
    std::ostringstream stream;
    stream << std::fixed;
    if (std::fabs(value - std::round(value)) < 1e-9) {
        stream << std::setprecision(1) << value;
    } else {
        stream << std::setprecision(6) << value;
        std::string text = stream.str();
        while (!text.empty() && text.back() == '0') {
            text.pop_back();
        }
        if (!text.empty() && text.back() == '.') {
            text.pop_back();
        }
        return text;
    }
    return stream.str();
}

}  // namespace

std::string formatTableLine(double sourceValue, const std::string& sourceUnit, double targetValue,
                            const std::string& targetUnit) {
    std::ostringstream stream;
    stream << formatNumber(sourceValue) << ' ' << sourceUnit << " = "
           << formatNumber(roundOneDecimal(targetValue)) << ' ' << targetUnit;
    return stream.str();
}

std::vector<std::string> formatTable(const std::string& sourceUnit, double sourceValue,
                                     const std::vector<entity::ConversionResult>& rows) {
    std::vector<std::string> lines;
    lines.reserve(rows.size());
    for (const auto& row : rows) {
        lines.push_back(formatTableLine(sourceValue, sourceUnit, row.value, row.targetUnit));
    }
    return lines;
}

}  // namespace boundary
