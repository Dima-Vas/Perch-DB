#pragma once

#include "PDataType.hpp"

class PDouble : public PDataType {
public:
    PDouble() : value{0.0} {}

    ~PDouble() override = default;

    PDouble(double aVal) : value{aVal} {}

    PDouble(PDouble&&) = delete;

    std::string valAsStr() const override {
        return std::to_string(value);
    }

    auto val() const noexcept {
        return value;
    }

private:
    const double value;
};