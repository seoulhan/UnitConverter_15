#pragma once

namespace entity {

enum class DomainError {
    kNonPositiveValue,
    kUnknownUnit,
    kDuplicateUnit,
    kInvalidFactor,
};

}  // namespace entity
