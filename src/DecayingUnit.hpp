#pragma once

#include <map>
#include <string>

#include "entity/unit_registry.hpp"

/// 동적 단위 등록·meter 허브 환산 (bonus). UnitConverter.cpp의 Item/분기는 수정하지 않는다.
class DecayingUnit {
public:
    DecayingUnit();

    void registerUnit(const std::string& unitId, double metersPerUnit);
    double convert(const std::string& fromUnit, double value, const std::string& toUnit) const;
    std::map<std::string, double> convertAll(const std::string& fromUnit, double value) const;

private:
    uc::entity::UnitRegistry registry_;
};
