#pragma once

#include <string>
#include <vector>

#include "entity/conversion_service.hpp"

namespace control {

class ConvertUseCase {
public:
    explicit ConvertUseCase(entity::ConversionService service);

    [[nodiscard]] std::vector<entity::ConversionResult> execute(const std::string& fromUnit,
                                                                double value) const;

private:
    entity::ConversionService service_;
};

}  // namespace control
