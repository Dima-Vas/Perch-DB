
#pragma once

#include "PDataType.hpp"
#include <string>

class PString : public PDataType {
public:
    PString() : value{nullptr} {};

    ~PString() override = default;

    PString(const std::string& aVal) : value{aVal} {}

    PString(PString&&) = delete;

    std::string valAsStr() const override {
        return value;
    }

    const std::string& val() const noexcept {
        return value;
    }

private:
    const std::string value;
};
