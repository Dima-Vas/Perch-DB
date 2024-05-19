#include <iostream>
#include <fstream>
#include <string>
#include "DataEngine.hpp"
#include "Table.hpp"

int main(int argc, char* argv[]) {
    try {
        DataEngine de{};
        const std::vector<std::string> v = {"Variable",
                                            "Breakdown",
                                            "Breakdown_category",
                                            "Year",
                                            "RD_Value",
                                            "Status",
                                            "Unit",
                                            "Footnotes",
                                            "Relative_Sampling_Error"};
        const std::string name = "wintest";
        const std::vector<PDataEnum> v2 = {PDataEnum::PSTRING,
                                    PDataEnum::PSTRING,
                                    PDataEnum::PSTRING,
                                    PDataEnum::PINTGR,
                                    PDataEnum::PINTGR,
                                    PDataEnum::PSTRING,
                                    PDataEnum::PSTRING,
                                    PDataEnum::PSTRING,
                                    PDataEnum::PDOUBLE};
        de.initTable(name, v, 10, 0, v2);
        Table t = de.getTable(name);
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 3;
    }
    return 0;
}
