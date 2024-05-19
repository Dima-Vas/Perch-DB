

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
#include "PDouble.hpp"
#include "ThreadPool.hpp"

#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include "linux/TableLinux.hpp"
#endif

#ifdef _WIN32
    #include "windows/TableWindows.hpp"
#endif

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
        std::cerr << "Bad fromMeta() with path " << path << " : " << strerror(errno) << std::endl;
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
                structure.push_back(PDataEnum::PINTGR);
            else if (type == "Str")
                structure.push_back(PDataEnum::PSTRING);
            else if (type == "Dbl")
                structure.push_back(PDataEnum::PDOUBLE);
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
    buffers = new RowBuffer[num_files];
    file.close();
    col_num = structure.size();
}

PDataType* formRowValue(std::string& entry, PDataEnum dataType) {
    switch (dataType) {
        case PDataEnum::PINTGR:
            return new PInt(std::stoi(entry));
        case PDataEnum::PSTRING:
            return new PString(entry);
        case PDataEnum::PDOUBLE:
            return new PDouble(std::stod(entry));
            break;
        default:
            throw std::runtime_error("Bad processFile() : PDataEnum not recognized");
    }
}


void Table::processFile(char* file_data, size_t  file_size, size_t  buff_idx) {
    std::string row;
    size_t  rows_read = 0;
    size_t  i = 0;
    std::ostringstream entry;
    bool in_quotes = false;
    PDataType** toSet = new PDataType*[col_num];
    DataRow* toPut;
    size_t  curr_col = 0;

    while (i < file_size && rows_read < MAXTABLEBUFFSIZE) {
        char ch = file_data[i++];
        if (in_quotes) {
            if (ch == '"') {
                in_quotes = false;
            } else {
                entry << ch;
            }
        } else {
            if (ch == '"') {
                in_quotes = true;
            } else if (ch == ',' || ch == '\n') {
                PDataEnum dataType = structure[curr_col];
                std::string entry_str = entry.str();
                toSet[curr_col] = formRowValue(entry_str, dataType);
                entry.str("");
                entry.clear();
                ++curr_col;
            } else {
                entry << ch;
            }
        }
        if (ch == '\n') {
            toPut = new DataRow{col_num};
            toPut->setData(toSet);
            buffers[buff_idx].addRow(toPut);
            ++i;
            ++rows_read;
            if (i < file_size && rows_read < MAXTABLEBUFFSIZE) {
                toSet = new PDataType*[col_num];
            }
            curr_col = 0;
            continue;
        }
    }
}

void Table::readBuffer(std::string& file_name, size_t  buff_idx) {
#ifdef __linux__
    readBufferLinux(file_name, buff_idx);
#endif
#ifdef _WIN32
    readBufferWindows(file_name, buff_idx);
#endif

}

/**
 * @brief Reads the next portion of table data to buffers.
 */
void Table::readBuffers() {
    std::vector<std::string> files;
    std::string name_str;
    size_t thread_num = 4;
    ThreadPool<decltype(&Table::readBuffer), Table*, std::string, size_t > tp{thread_num, &Table::readBuffer};
    try {
        for (size_t  i = 0; i < num_files; ++i) {
            std::ostringstream filepath;
            filepath << basepath << i + 1  << ".pdb";
            name_str = filepath.str();
            files.push_back(name_str);
        }
        std::vector<std::thread> thr_vec;
        for (size_t  i = 0; i < num_files; ++i) {
            tp.enqueue(this, std::move(files[i]), std::move(i));
        }
    } catch (const std::exception& e) {
        std::cerr << "Bad readBuffers() : failure to open table file : " << e.what() << std::endl;
        std::cerr << "This is most likely a bug in PDB implementation" << std::endl;
        throw;
    }
    for (size_t i = 0; i < thread_num; ++i) {
        tp.enqueue(nullptr, "", -1ull);
    }
}

void Table::readBuffersSeq() {
    std::string name_str;
    try {
        for (size_t  i = 0; i < num_files; ++i) {
            std::ostringstream filepath;
            filepath << basepath << i + 1  << ".pdb";
            name_str = filepath.str();
            readBuffer(name_str, i);
        }
    } catch (const std::exception& e) {
        std::cerr << "Bad readBuffersSeq() : " << e.what() << std::endl;
        std::cerr << "This is most likely a bug in PDB implementation" << std::endl;
        throw;
    }
}
