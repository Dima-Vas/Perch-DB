
#pragma once

#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "Table.hpp"

void Table::readBufferLinux(std::string& file_name, size_t buff_idx) {
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Bad open() in readBufferLinux(): " << file_name << " : " << strerror(errno) << std::endl;
        throw std::runtime_error("Bad open() in readBufferLinux()");
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        std::cerr << "Bad fstat() in readBufferLinux(): " << file_name << " : " << strerror(errno) << std::endl;
        throw std::runtime_error("Bad fstat() in readBufferLinux():");
    }
    size_t  file_size = sb.st_size;
    if (file_size == 0) {
        close(fd);
        return;
    }
    char* file_data = static_cast<char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_data == MAP_FAILED) {
        close(fd);
        std::cerr << "Bad mmap() in readBufferLinux() : " << file_name << " : " << strerror(errno) << std::endl;
        throw std::runtime_error("Bad mmap() in readBufferLinux()");
    }
    processFile(file_data, file_size, buff_idx);
    munmap(file_data, file_size);
    close(fd);
}
