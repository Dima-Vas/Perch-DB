#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "DataEngine.hpp"
#include "Table.hpp"

int main(int argc, char* argv[]) {
    DataEngine de{};
    std::cout << "Starting to measure..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    Table t = de.getTable("wintest");
    PDataType** data_ptr = t.getRow(1000)->getData();
    size_t last_col = t.getStructure().size() - 1;
    switch (t.getStructure()[last_col]) {
        case PDataEnum::PINTGR :
            std::cerr << dynamic_cast<PInt*>(data_ptr[last_col])->val() << std::endl;
            break;
        case PDataEnum::PSTRING:
            std::cerr << dynamic_cast<PString*>(data_ptr[last_col])->val() << std::endl;
            break;
        case PDataEnum::PDOUBLE:
            std::cerr << dynamic_cast<PDouble*>(data_ptr[last_col])->val() << std::endl;
            break;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
    std::cout << "Rows read : " << t.getRowsNum() << std::endl;
    return 0;
}
