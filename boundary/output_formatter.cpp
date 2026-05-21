#include "boundary/output_formatter.hpp"

#include <iomanip>
#include <sstream>

namespace uc::boundary {

namespace {

std::string formatTargetValue(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(6) << value;
    return out.str();
}

}  // namespace

std::vector<std::string> OutputFormatter::formatTable(
    const std::string& sourceValueToken,
    const std::string& sourceUnit,
    const std::vector<uc::entity::ConversionLine>& lines) {
    std::vector<std::string> formatted;
    formatted.reserve(lines.size());
    for (const auto& line : lines) {
        std::ostringstream row;
        row << sourceValueToken << ' ' << sourceUnit << " = "
            << formatTargetValue(line.targetValue) << ' ' << line.targetUnit;
        formatted.push_back(row.str());
    }
    return formatted;
}

}  // namespace uc::boundary
