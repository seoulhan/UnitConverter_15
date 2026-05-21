#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "helpers/approval_tests.hpp"

#ifndef UC_SOURCE_DIR
#define UC_SOURCE_DIR "."
#endif

#ifndef UC_EXE_PATH
#define UC_EXE_PATH "unit_converter"
#endif

#ifndef UC_TEST_WORK_DIR
#define UC_TEST_WORK_DIR "."
#endif

// Catch2 TEST_CASE_METHOD maps to Google Test TEST_F(GoldenMasterTest, ...) pattern.
class GoldenMasterTest {
public:
    GoldenMasterTest()
        : sourceRoot_(UC_SOURCE_DIR),
          workDir_(UC_TEST_WORK_DIR),
          exePath_(UC_EXE_PATH),
          baselinePath_(sourceRoot_ / "tests" / "golden_master_expected.txt"),
          inputPath_(workDir_ / "input.txt"),
          actualPath_(workDir_ / "actual.txt") {}

protected:
    void runCase(const char* caseId,
                 const char* inputLine,
                 const char* sectionTag) {
        INFO("Case ID: " << caseId);
        REQUIRE(std::filesystem::exists(exePath_));

        REQUIRE(uc::approval::captureStdoutToFile(
            exePath_, inputLine, inputPath_, actualPath_));

        const std::string expectedStr =
            uc::approval::extractSection(baselinePath_, sectionTag);

        const std::string actualStr = uc::approval::stripPromptLines(
            uc::approval::readFile(actualPath_));
        EXPECT_EQ(expectedStr, actualStr);
    }

private:
    std::filesystem::path sourceRoot_;
    std::filesystem::path workDir_;
    std::filesystem::path exePath_;
    std::filesystem::path baselinePath_;
    std::filesystem::path inputPath_;
    std::filesystem::path actualPath_;
};

TEST_CASE_METHOD(GoldenMasterTest,
                 "GM-TC-01 UnitConverter_meter_2_5",
                 "[golden_master][GM-TC-01]") {
    runCase("GM-TC-01", "meter:2.5", "meter:2.5");
}

TEST_CASE_METHOD(GoldenMasterTest,
                 "GM-TC-02 UnitConverter_feet_1_0",
                 "[golden_master][GM-TC-02]") {
    runCase("GM-TC-02", "feet:1.0", "feet:1.0");
}

TEST_CASE_METHOD(GoldenMasterTest,
                 "GM-TC-03 UnitConverter_yard_1_0",
                 "[golden_master][GM-TC-03]") {
    runCase("GM-TC-03", "yard:1.0", "yard:1.0");
}

TEST_CASE_METHOD(GoldenMasterTest,
                 "GM-TC-04 UnitConverter_meter_0_0",
                 "[golden_master][GM-TC-04]") {
    runCase("GM-TC-04", "meter:0.0", "meter:0.0");
}
