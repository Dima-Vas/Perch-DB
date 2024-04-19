

#include <fstream>
#include <sstream>
#include <iosfwd>
#include <cstring>
#include <string>
#include <thread>
#include <functional>

#include "defs.h"
#include "Table.hpp"
#include "PInt.hpp"
#include "PString.hpp"


/**
 * @brief initializes the Table from the metafile path
 * 
 * 
 * @param path 
 * @return
 */
void Table::fromMeta(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Bad fromMeta() : " << strerror(errno) << std::endl;
        throw std::runtime_error("Bad fromMeta()");
    }
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> name;
    }
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        while (iss >> type) {
            if (type == "Int")
                structure.push_back(PDataEnum::INT);
            else if (type == "Str")
                structure.push_back(PDataEnum::STRING);
        }
    }
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string col_name;
        while (iss >> col_name) {
            col_names.push_back(col_name);
        }
    }
    if (!std::getline(file, line)) {
        std::cerr << "Bad fromMeta() : could not read the primary key" << std::endl;
        file.close();
        throw std::runtime_error("Bad fromMeta()");
    } else {
        pk_col_idx = find(col_names.begin(), col_names.end(), line) - col_names.begin();
    }
    if (!std::getline(file, line)) {
        std::cerr << "Bad fromMeta() : could not read the files number" << std::endl;
        file.close();
        throw std::runtime_error("Bad fromMeta()");
    } else {
        num_files = atoi(line.c_str());
    }
    file.close();
    buffers = new RowBuffer[num_files];
    col_num = structure.size();
}

void Table::readBuffer(std::ifstream& file, size_t buff_idx) {
    size_t rows_read = 0;
    std::string row;
    std::size_t i;
    while (rows_read < MAXTABLEBUFFSIZE && std::getline(file, row)) {
        PDataType** toSet = new PDataType*[col_num];
        DataRow* toPut = new DataRow{col_num};
        std::istringstream ss(row);
        std::string token;
        i = 0;
        bool in_quotes = false;
        std::string entry;
        while (std::getline(ss, token, ',')) {
            if (in_quotes) {
                entry += "," + token;
                if (token.back() == '"' || token.back() == '\r') {
                    entry = entry.substr(1, entry.size() - 2);  // Remove leading and trailing quotes
                    in_quotes = false;
                }
            } else {
                if (token.front() == '"') {
                    entry = token.substr(1);
                    in_quotes = true;
                } else {
                    entry = token;
                }
            }
            if (!in_quotes) {
                PDataEnum dataType = structure[i];
                if (dataType == PDataEnum::INT) {
                    toSet[i] = new PInt(stoi(entry));
                } else if (dataType == PDataEnum::STRING) {
                    toSet[i] = new PString(entry);
                }
                ++i;
            }
        }
        toPut->setData(toSet);
        buffers[buff_idx].addRow(toPut);
        ++rows_read;
    }
    file.close();
}

/**
 * @brief Reads the next portion of table data to buffers.
 */
void Table::readBuffers() { // TODO : threadpool
    std::vector<std::ifstream> files;
    std::string name_str;
    try {
        for (size_t i = 0; i < num_files; ++i) {
            std::ostringstream filepath;
            filepath << basepath << i + 1  << ".pdb";
            name_str = filepath.str();
            files.push_back(std::move(std::ifstream(name_str)));
            if (!files[i].is_open()){
                throw std::runtime_error("Bad readBuffers()");
            }
        }
        std::vector<std::thread> thr_vec;
        for (size_t i = 0; i < num_files; ++i) {
            thr_vec.push_back(std::thread(&Table::readBuffer, this, std::ref(files[i]), i));
        }
        for (auto& thr : thr_vec) {
            thr.join();
        }
    } catch (const std::exception& e) {
        for (auto& file : files) {
            file.close();
        }
        std::cerr << "Bad readBuffers() : failure to open table file : " << strerror(errno) << std::endl;
        std::cerr << "This is most likely a bug in PDB implementation" << std::endl;
        throw;
    }
}

void Table::readBuffersSeq() {
    std::vector<std::ifstream> files;
    std::string name_str;
    try {
        for (size_t i = 0; i < num_files; ++i) {
            std::ostringstream filepath;
            filepath << basepath << i + 1  << ".pdb";
            name_str = filepath.str();
            std::ifstream file(name_str);
            if (!file.is_open()) {
                throw std::runtime_error("Bad readBuffers()");
            }
            readBuffer(file, i);
            file.close();
        }
    } catch (const std::exception& e) {
        for (auto& file : files) {
            file.close();
        }
        std::cerr << "Bad readBuffers() : failure to open table file : " << strerror(errno) << std::endl;
        std::cerr << "This is most likely a bug in PDB implementation" << std::endl;
        throw;
    }
}
