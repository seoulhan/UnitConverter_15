#pragma once

#include <catch2/catch_test_macros.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace uc::approval {

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

inline std::string trimTrailingNewlines(std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
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
    for (char c : path.string()) {
        if (c == '"') {
            quoted += "\\\"";
        } else {
            quoted += c;
        }
    }
    quoted += "\"";
    return quoted;
}

inline std::string stripPromptLines(std::string captured) {
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

inline bool captureStdoutToFile(const std::filesystem::path& exe,
                                const std::string& inputLine,
                                const std::filesystem::path& inputPath,
                                const std::filesystem::path& actualPath) {
    writeFile(inputPath, inputLine + "\n");

    std::ostringstream command;
#if defined(_WIN32)
    command << "cmd /C " << shellQuote(exe) << " < " << shellQuote(inputPath)
            << " > " << shellQuote(actualPath) << " 2>NUL";
#else
    command << shellQuote(exe) << " < " << shellQuote(inputPath) << " > "
            << shellQuote(actualPath) << " 2>/dev/null";
#endif

    std::system(command.str().c_str());
    return std::filesystem::exists(actualPath);
}

inline std::string extractSection(const std::filesystem::path& baselinePath,
                                 const std::string& sectionTag) {
    const auto document = readFile(baselinePath);
    const std::string header = '[' + sectionTag + ']';
    const auto start = document.find(header);
    if (start == std::string::npos) {
        return {};
    }

    auto bodyStart = start + header.size();
    if (bodyStart < document.size() && document[bodyStart] == '\n') {
        ++bodyStart;
    }

    if (bodyStart + 3 <= document.size() &&
        document.compare(bodyStart, 3, "---") == 0) {
        return {};
    }

    const auto sectionEnd = document.find("\n---", bodyStart);
    const auto body = document.substr(
        bodyStart,
        sectionEnd == std::string::npos ? std::string::npos : sectionEnd - bodyStart);
    return trimTrailingNewlines(body);
}

inline std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

inline std::string unifiedDiff(const std::string& expected,
                               const std::string& actual,
                               const std::string& expectedLabel = "expected",
                               const std::string& actualLabel = "actual") {
    const auto expLines = splitLines(expected);
    const auto actLines = splitLines(actual);

    std::ostringstream diff;
    diff << "--- " << expectedLabel << '\n';
    diff << "+++ " << actualLabel << '\n';

    const std::size_t maxLines = std::max(expLines.size(), actLines.size());
    std::size_t i = 0;
    while (i < maxLines) {
        const bool same = i < expLines.size() && i < actLines.size() &&
                          expLines[i] == actLines[i];
        if (same) {
            ++i;
            continue;
        }

        const std::size_t hunkStart = i + 1;
        std::size_t expEnd = i;
        std::size_t actEnd = i;
        while (expEnd < expLines.size() || actEnd < actLines.size()) {
            const bool expDone = expEnd >= expLines.size();
            const bool actDone = actEnd >= actLines.size();
            if (!expDone && !actDone && expLines[expEnd] == actLines[actEnd]) {
                break;
            }
            if (expEnd < expLines.size()) {
                ++expEnd;
            }
            if (actEnd < actLines.size()) {
                ++actEnd;
            }
            if (expDone && actDone) {
                break;
            }
        }

        const std::size_t expCount = expEnd - i;
        const std::size_t actCount = actEnd - i;
        diff << "@@ -" << hunkStart << ',' << expCount << " +" << hunkStart << ','
             << actCount << " @@\n";

        for (std::size_t e = i; e < expEnd; ++e) {
            diff << '-' << expLines[e] << '\n';
        }
        for (std::size_t a = i; a < actEnd; ++a) {
            diff << '+' << actLines[a] << '\n';
        }
        i = std::max(expEnd, actEnd);
    }

    return diff.str();
}

inline void expectEq(const std::string& expected,
                     const std::string& actual,
                     const std::string& caseId) {
    if (expected == actual) {
        return;
    }

    const auto diff = unifiedDiff(expected, actual);
    INFO("Case: " << caseId);
    UNSCOPED_INFO(diff);
    FAIL_CHECK("EXPECT_EQ failed for " << caseId);
}

}  // namespace uc::approval

#define EXPECT_EQ(expected, actual) \
    do { \
        const std::string& _expected_ref = (expected); \
        const std::string& _actual_ref = (actual); \
        uc::approval::expectEq(_expected_ref, _actual_ref, #expected " == " #actual); \
    } while (false)
