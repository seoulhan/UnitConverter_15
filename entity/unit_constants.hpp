#pragma once

namespace entity {

// 1 meter = 3.28084 feet  => meters per 1 foot = 0.3048
constexpr double kMetersPerMeter = 1.0;
constexpr double kMetersPerFoot = 0.3048;
constexpr double kMetersPerYard = 0.9144;
constexpr double kFeetPerMeter = 3.28084;
constexpr double kYardsPerMeter = 1.09361;
constexpr double kDefaultEpsilon = 1e-5;

}  // namespace entity
