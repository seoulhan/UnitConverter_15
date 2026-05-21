#include "DecayingUnit.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

namespace {

constexpr double kEps = 1e-5;
constexpr double kCubitMeters = 0.4572;
constexpr double kFeetPerMeter = 3.28084;

DecayingUnit makeWithCubit() {
    DecayingUnit du;
    du.registerUnit("cubit", kCubitMeters);
    return du;
}

}  // namespace

TEST_CASE("감쇠 단위 변환", "[bonus]") {
    SECTION("BT-01 registerUnit cubit then cubit to meter") {
        const DecayingUnit du = makeWithCubit();
        REQUIRE(du.convert("cubit", 1.0, "meter") == Catch::Approx(0.4572).margin(kEps));
    }

    SECTION("BT-02 registerUnit then meter to cubit inverse") {
        const DecayingUnit du = makeWithCubit();
        const double expected = 1.0 / kCubitMeters;
        REQUIRE(du.convert("meter", 1.0, "cubit") == Catch::Approx(expected).margin(kEps));
    }

    SECTION("BT-03 registerUnit then cubit to feet cross conversion") {
        const DecayingUnit du = makeWithCubit();
        const double expected = kCubitMeters * kFeetPerMeter;
        REQUIRE(du.convert("cubit", 1.0, "feet") == Catch::Approx(expected).margin(kEps));
    }

    SECTION("BT-04 negative ratio registration throws invalid_argument") {
        DecayingUnit du;
        REQUIRE_THROWS_AS(du.registerUnit("bad", -1.0), std::invalid_argument);
    }

    SECTION("BT-05 registerUnit then convertAll returns all units") {
        const DecayingUnit du = makeWithCubit();
        const auto all = du.convertAll("cubit", 1.0);

        REQUIRE(all.size() == 4);
        REQUIRE(all.at("meter") == Catch::Approx(kCubitMeters).margin(kEps));
        REQUIRE(all.at("feet") == Catch::Approx(kCubitMeters * kFeetPerMeter).margin(kEps));
        REQUIRE(all.at("yard") ==
                Catch::Approx(kCubitMeters * 1.09361).margin(kEps));
        REQUIRE(all.at("cubit") == Catch::Approx(1.0).margin(kEps));
    }

    SECTION("BT-06 legacy meter to feet unchanged regression") {
        DecayingUnit du;
        REQUIRE(du.convert("meter", 1.0, "feet") ==
                Catch::Approx(kFeetPerMeter).margin(kEps));
    }
}
