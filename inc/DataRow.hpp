
#pragma once

#ifdef __linux__
    #include <cstdlib>
#endif
#include <array>
#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "PDataType.hpp"
#include "PInt.hpp"
#include "PString.hpp"

// std::array<DataRow, std::atol(::getenv("PDB_BUFFSIZE"))> ROWBUFFER;

class DataRow {
public:
    DataRow() = delete;

    ~DataRow() {
        for (size_t  i = 0; i < len; ++i) {
            delete data[i];
        }
        delete[] data;
    };

    DataRow(size_t  N) : len{N} {}

    size_t  size() const noexcept {
        return len;
    }

    PDataType** getData() const noexcept{
        return data;
    }

    void setData(PDataType** toSet) {
        data = toSet;
    }

    std::string strValAt(size_t  idx) const {
        size_t  num_rows = size();
        if (idx >= num_rows) {
            std::cerr << "Bad idx in DataRow's strValAt()" << num_rows << std::endl;
            throw std::runtime_error{"Bad strValAt() in DataRow : idx out of range"};
        }
        return (*data[idx]).valAsStr();
    }

    void setNextRow(DataRow* aNext) noexcept {
        next = aNext;
    }

    DataRow* getNextRow() const noexcept {
        return next;
    }

private:
    PDataType** data;
    size_t  len;
    DataRow* next;
};
