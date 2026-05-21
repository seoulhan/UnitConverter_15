#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <vector>

#include "helpers/golden_master.hpp"

#ifndef UC_SOURCE_DIR
#define UC_SOURCE_DIR "."
#endif

#ifndef UC_EXE_PATH
#define UC_EXE_PATH "unit_converter"
#endif

namespace {

std::filesystem::path sourceRoot() {
    return std::filesystem::path(UC_SOURCE_DIR);
}

std::filesystem::path converterExecutable() {
    return std::filesystem::path(UC_EXE_PATH);
}

std::filesystem::path expectedPath() {
    return sourceRoot() / "tests" / "golden_master_expected.txt";
}

const std::vector<std::string>& scenarios() {
    static const std::vector<std::string> kScenarios = {
        "meter:2.5",
        "feet:1.0",
        "yard:1.0",
        "meter:0.0",
    };
    return kScenarios;
}

}  // namespace

TEST_CASE("Golden master approval regression", "[integration][golden_master][regression]") {
    const auto exe = converterExecutable();
    INFO("Executable: " << exe.string());
    REQUIRE(std::filesystem::exists(exe));

    const auto actual = uc::golden_master::buildActualDocument(exe, scenarios());
    REQUIRE_FALSE(actual.empty());

    const auto expectedFile = expectedPath();
    if (!std::filesystem::exists(expectedFile)) {
        REQUIRE(uc::golden_master::writeFile(expectedFile, actual));
        WARN("Created baseline at tests/golden_master_expected.txt — run: git add tests/golden_master_expected.txt");
        return;
    }

    const auto expected = uc::golden_master::trimTrailingNewlines(
        uc::golden_master::readFile(expectedFile));
    const auto normalizedActual = uc::golden_master::trimTrailingNewlines(actual);

    if (normalizedActual != expected) {
        const auto diff = uc::golden_master::diffLines(expected, normalizedActual);
        UNSCOPED_INFO("Golden master diff:\n" << diff);
        FAIL("Golden master mismatch — update baseline only after intentional output change");
    }
}
