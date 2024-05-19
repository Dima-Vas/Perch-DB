


#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <optional>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <string_view>

enum QueryToken {
    CREATE,
    DATABASE,
    IF
};

class QueryParser {
private:
    QueryParser() {}

    bool isReadingVal = false;
	static constexpr int myhash(std::string_view str) noexcept {
        unsigned int hash = 5381;
        for (char c : str)
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        return static_cast<int>(hash);
    }

public:
    ~QueryParser() = default;

    QueryParser(const QueryParser&) = delete;

    static QueryParser& getParserInstance() {
        static QueryParser instance;
        return instance;
    }

	QueryParser& operator=(const QueryParser&) = delete;


    // SELECT * FROM database.table;
    // INSERT INTO database.table("x", 'y') VALUES (3, "Shalom");
    // SELECT x,y FROM database.table WHERE x > 2;
    // UPDATE database.table SET x = 2 WHERE x > 2;
    // DELETE FROM database.table WHERE x = 3;
    // CREATE TABLE database.table(x INTEGER, column2 VARCHAR(50) NOT NULL, PRIMARY KEY(x));

    void processQuery(const std::string& query) {
        std::stringstream currWordBuffer;
        for (char curr : query) {
            if (curr == '\'') {
                if (isReadingVal) {
                    std::cout << "Read the value :" << currWordBuffer.str() << std::endl;
                    currWordBuffer.str("");
                }
                isReadingVal ^= 1;
                continue;
            }
            if (curr == ' ' && !isReadingVal) {
                continue;
            }
            currWordBuffer << curr;

            if (isReadingVal)
                continue;
            std::optional<QueryToken> token = checkIfToken(currWordBuffer);
            if (token.has_value()) {
                currWordBuffer.str("");
            }
        }
    }

    std::optional<QueryToken> checkIfToken(std::stringstream& bufferToCheck) {
		std::string bStr = bufferToCheck.str();
		std::transform(bStr.begin(), bStr.end(), bStr.begin(),
							[](unsigned char c){return std::tolower(c);});
        static constexpr int createHash = myhash("create");
        static constexpr int databaseHash = myhash("database");
        static constexpr int ifHash = myhash("if");
        int hashRes = myhash(bStr.c_str());
		switch(hashRes) {
			case createHash :
                std::cout << "Found CREATE" << std::endl;
				return std::optional<QueryToken>(QueryToken::CREATE);
            case databaseHash :
                std::cout << "Found DATABASE" << std::endl;
                return std::optional<QueryToken>(QueryToken::DATABASE);
		    case ifHash :
                std::cout << "Found IF" << std::endl;
                return std::optional<QueryToken>(QueryToken::IF);
        };
		return std::nullopt;
    }
};
