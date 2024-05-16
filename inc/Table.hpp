

#pragma once

#include <string>
#include <array>
#include <memory>
#include "PDataType.hpp"
#include "PDataEnum.hpp"
#include "DataRow.hpp"

typedef struct RowBuffer {
    DataRow* first_row = nullptr;
    size_t rows_read = 0;
    DataRow* last_row = nullptr;

    void clearBuffer() noexcept {
        DataRow* next;
        DataRow* curr = first_row;
        while (rows_read != 0) {
            next = curr->getNextRow();
            delete curr;
            curr = next;
            --rows_read;
        }
    }

    DataRow* getRow(size_t idx) {
        DataRow* curr = first_row;
        while (idx != 0 && curr != nullptr) {
            curr = curr->getNextRow();
            --idx;
        }
        if (curr == nullptr) {
            std::cerr << "Bad getRow() : row index out of range" << std::endl;
            throw std::runtime_error("Bad getRow()");
        }
        return curr;
    }

    void addRow(DataRow* newRow) {
        if (first_row == nullptr) {
            first_row = newRow;
            last_row = newRow;
        } else {
            last_row->setNextRow(newRow);
            last_row = newRow;
        }
        ++rows_read;
    }
} RowBuffer;

/**
 * @brief Used to represent current table in the memory
 *  
 * 
 */
class Table {
public:
    Table() = delete;

    ~Table() {
        for (size_t i = 0; i < num_files; ++i) {
            buffers[i].clearBuffer();
        }
        delete[] buffers;
    }

    Table(const std::string& aBasepath) {
        basepath = aBasepath;
        fromMeta(basepath + "/meta.pdb");  
    }

    void readBuffers();

    void readBuffersSeq();

    RowBuffer* getBuffer(size_t idx) const { // getter
        if (idx >= num_files) {
            std::cerr << "Bad getBuffer() : idx out of range" << std::endl;
            throw std::out_of_range("Bad getBuffer()");
        }
        return buffers + idx;
    }

    const std::vector<PDataEnum>& getStructure() const noexcept { // getter
        return structure;
    }

private:
    std::string name;
    std::vector<PDataEnum> structure;
    std::vector<std::string> col_names;
    size_t pk_col_idx;
    uint8_t num_files; // max 255 data files per table
    RowBuffer* buffers;
    std::string basepath;
    size_t row_num;
    size_t col_num;

    void fromMeta(const std::string& path);

    void readBuffer(std::string& file, size_t buff_idx);

    void processFile(char* file_data, size_t file_size, size_t buff_idx);
};
