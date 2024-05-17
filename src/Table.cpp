

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

#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
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
    buffers = new RowBuffer[num_files];
    file.close();
    col_num = structure.size();
}

void Table::processFile(char* file_data, size_t file_size, size_t buff_idx) {
    std::string row;
    size_t rows_read = 0;
    size_t i = 0;
    std::ostringstream entry;
    bool in_quotes = false;
    PDataType** toSet = new PDataType*[col_num];
    DataRow* toPut;
    size_t curr_col = 0;

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
                std::string x = entry.str();
                if (dataType == PDataEnum::INT) {
                    toSet[curr_col] = new PInt(stoi(entry.str()));
                } else if (dataType == PDataEnum::STRING) {
                    toSet[curr_col] = new PString(entry.str());
                }
                entry.str("");
                entry.clear();
                x = entry.str();
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

void Table::readBuffer(std::string& file_name, size_t buff_idx) {
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Bad open() in readBuffer(): " << file_name << strerror(errno) << std::endl;
        throw std::runtime_error("Bad open() in readBuffer()");
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        std::cerr << "Bad fstat() in readBuffer(): " << file_name << strerror(errno) << std::endl;
        throw std::runtime_error("Bad fstat() in readBuffer():");
    }
    size_t file_size = sb.st_size;
    char* file_data = static_cast<char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_data == MAP_FAILED) {
        close(fd);
        std::cerr << "Bad mmap() in readBuffer() : " << file_name << strerror(errno) << std::endl;
        throw std::runtime_error("Bad mmap() in readBuffer()");
    }
    processFile(file_data, file_size, buff_idx);
}

/**
 * @brief Reads the next portion of table data to buffers.
 */
void Table::readBuffers() { // TODO : threadpool
    std::vector<std::string> files;
    std::string name_str;
    try {
        for (size_t i = 0; i < num_files; ++i) {
            std::ostringstream filepath;
            filepath << basepath << i + 1  << ".pdb";
            name_str = filepath.str();
            files.push_back(name_str);
        }
        std::vector<std::thread> thr_vec;
        for (size_t i = 0; i < num_files; ++i) {
            thr_vec.push_back(std::thread(&Table::readBuffer, this, std::ref(files[i]), i));
        }
        for (auto& thr : thr_vec) {
            thr.join();
        }
    } catch (const std::exception& e) {
        std::cerr << "Bad readBuffers() : failure to open table file : " << e.what() << std::endl;
        std::cerr << "This is most likely a bug in PDB implementation" << std::endl;
        throw;
    }
}

void Table::readBuffersSeq() {
    std::string name_str;
    try {
        for (size_t i = 0; i < num_files; ++i) {
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
