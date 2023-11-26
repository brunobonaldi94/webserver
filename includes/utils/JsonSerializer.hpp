#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>

class JsonSerializer {
    public:
        static std::string serialize(const std::map<std::string, std::string>& data);
        static std::map<std::string, std::string> deserialize(const std::string& jsonStr);
        static void save(std::string data);
};

