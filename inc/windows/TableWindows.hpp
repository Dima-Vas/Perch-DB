
#pragma once

#include <windows.h>
#include <string>
#include <stdexcept>
#include <iostream>

void Table::readBufferWindows(std::string& file_name, size_t buff_idx) {
    HANDLE hFile = CreateFileA(
        file_name.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Bad CreateFile() in readBuffer(): " << file_name << strerror(errno) << std::endl;
        throw std::runtime_error("Bad CreateFile() in readBuffer()");
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        CloseHandle(hFile);
        std::cerr << "Bad GetFileSizeEx() in readBuffer(): " << file_name << strerror(errno) << std::endl;
        throw std::runtime_error("Bad GetFileSizeEx() in readBuffer()");
    }
    if (fileSize.QuadPart == 0) {
        CloseHandle(hFile);
        return;
    }
    HANDLE hMapping = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READONLY,
        fileSize.HighPart,
        fileSize.LowPart,
        NULL
    );
    if (!hMapping) {
        CloseHandle(hFile);
        std::cerr << "Bad CreateFileMapping() in readBuffer(): " << file_name << " : " << strerror(GetLastError()) << std::endl;
        throw std::runtime_error("Bad CreateFileMapping() in readBuffer()");
    }

    char* file_data = static_cast<char*>(MapViewOfFile(
        hMapping,
        FILE_MAP_READ,
        0,
        0,
        fileSize.QuadPart
    ));
    if (!file_data) {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        std::cerr << "Bad MapViewOfFile() in readBuffer(): " << file_name << GetLastError() << std::endl;
        throw std::runtime_error("Bad MapViewOfFile() in readBuffer()");
    }

    processFile(file_data, fileSize.QuadPart, buff_idx);

    UnmapViewOfFile(file_data);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}
