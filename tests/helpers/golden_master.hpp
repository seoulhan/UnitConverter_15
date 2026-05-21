#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace uc::golden_master {

inline std::string trimTrailingNewlines(std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

inline std::string normalizeNewlines(std::string s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n') {
            out.push_back('\n');
            ++i;
        } else if (s[i] == '\r') {
            out.push_back('\n');
        } else {
            out.push_back(s[i]);
        }
    }
    return out;
}

inline std::string readFile(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return normalizeNewlines(buffer.str());
}

inline bool writeFile(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        return false;
    }
    out << content;
    if (!content.empty() && content.back() != '\n') {
        out << '\n';
    }
    return static_cast<bool>(out);
}

inline std::string shellQuote(const std::filesystem::path& path) {
    std::string quoted = "\"";
    const auto native = path.string();
    for (char c : native) {
        if (c == '"') {
            quoted += "\\\"";
        } else {
            quoted += c;
        }
    }
    quoted += "\"";
    return quoted;
}

inline std::string captureStdout(const std::filesystem::path& exe,
                                 const std::string& inputLine) {
    const auto tempDir = std::filesystem::temp_directory_path();
    const auto inputPath = tempDir / "uc_golden_input.txt";
    const auto outputPath = tempDir / "uc_golden_actual.txt";

    writeFile(inputPath, inputLine + "\n");

    std::ostringstream command;
#if defined(_WIN32)
    command << "cmd /C " << shellQuote(exe) << " < " << shellQuote(inputPath)
            << " > " << shellQuote(outputPath) << " 2>NUL";
#else
    command << shellQuote(exe) << " < " << shellQuote(inputPath) << " > "
            << shellQuote(outputPath) << " 2>/dev/null";
#endif

    std::system(command.str().c_str());

    auto captured = readFile(outputPath);
    std::istringstream lines(captured);
    std::string line;
    std::ostringstream filtered;
    while (std::getline(lines, line)) {
        if (line.rfind("Insert value for converting", 0) == 0) {
            continue;
        }
        if (!line.empty() || filtered.tellp() > 0) {
            filtered << line << '\n';
        }
    }
    return trimTrailingNewlines(filtered.str());
}

inline std::string buildActualDocument(const std::filesystem::path& exe,
                                       const std::vector<std::string>& scenarios) {
    std::ostringstream document;
    for (std::size_t i = 0; i < scenarios.size(); ++i) {
        document << '[' << scenarios[i] << "]\n";
        const auto body = captureStdout(exe, scenarios[i]);
        if (!body.empty()) {
            document << body << '\n';
        }
        document << "---";
        if (i + 1 < scenarios.size()) {
            document << '\n';
        }
    }
    return document.str() + "\n";
}

inline std::string diffLines(const std::string& expected, const std::string& actual) {
    std::istringstream expStream(expected);
    std::istringstream actStream(actual);
    std::string expLine;
    std::string actLine;
    std::ostringstream diff;
    std::size_t lineNo = 1;
    bool hasDiff = false;

    while (true) {
        const bool hasExp = static_cast<bool>(std::getline(expStream, expLine));
        const bool hasAct = static_cast<bool>(std::getline(actStream, actLine));
        if (!hasExp && !hasAct) {
            break;
        }
        if (expLine != actLine) {
            hasDiff = true;
            diff << lineNo << ": expected: " << (hasExp ? expLine : "<EOF>") << '\n';
            diff << lineNo << ": actual  : " << (hasAct ? actLine : "<EOF>") << '\n';
        }
        ++lineNo;
    }

    if (!hasDiff) {
        return {};
    }
    return diff.str();
}

}  // namespace uc::golden_master
