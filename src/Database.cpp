
#pragma once

#include <map>
#include <iostream>

#include "DataRow.hpp"

class Database {
public:
    Database() : name{}, root_path{}, row_num{} {}

    ~Database() {}

    void addData(const std::string& key, const std::string& value) {
        data[key] = value;
    }

    std::string getData(const std::string& key) const {
        if (data.find(key) != data.end()) {
            return data.at(key);
        } else {
            return "Key not found.";
        }
    }

private:
    std::string name;
    std::string root_path;
    size_t  row_num;
    size_t  col_num;

    std::map<RecordID, DataRow> cache_data;
};


