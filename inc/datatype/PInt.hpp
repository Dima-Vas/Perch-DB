
#pragma once

#include "PDataType.hpp"

class PInt : public PDataType {
public:
    PInt() : value{0} {}

    ~PInt() override = default;

    PInt(int aVal) : value{aVal} {}

    PInt(PInt&&) = delete;

    std::string valAsStr() const override {
        return std::to_string(value);
    }

    auto val() const noexcept {
        return value;
    }

private:
    const int value;
};
