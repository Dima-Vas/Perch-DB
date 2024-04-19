

#pragma once

#include <string>

// TODO : get rid of virtual functions in favor of templates
class PDataType {
public:
    PDataType() = default;

    virtual ~PDataType() {};

    // PDataType(PDataType&&) = delete;

    virtual std::string valAsStr() const = 0;
private:
};
