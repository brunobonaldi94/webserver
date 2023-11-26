#include "JsonSerializer.hpp"

std::string JsonSerializer::serialize(const std::map<std::string, std::string>& data) {
    std::string jsonStr = "{";

    for (std::map<std::string, std::string>::const_iterator it = data.begin(); it != data.end(); ++it)
        jsonStr += "\"" + it->first + "\":\"" + it->second + "\",";
    if (!data.empty())
        jsonStr.erase(jsonStr.length() - 1);
    jsonStr += "}";
    return jsonStr;
}

std::map<std::string, std::string> JsonSerializer::deserialize(const std::string& jsonStr) {
    std::map<std::string, std::string> result;

    size_t pos = 1; 
    while (pos < jsonStr.length() - 1) {
        size_t keyStart = jsonStr.find("\"", pos);
        size_t keyEnd = jsonStr.find("\"", keyStart + 1);
        size_t valueStart = jsonStr.find("\"", keyEnd + 1);
        size_t valueEnd = jsonStr.find("\"", valueStart + 1);

        std::string key = jsonStr.substr(keyStart + 1, keyEnd - keyStart - 1);
        std::string value = jsonStr.substr(valueStart + 1, valueEnd - valueStart - 1);

        result[key] = value;
        pos = valueEnd + 2; 
    }
    return result;
}

void JsonSerializer::save(std::string data)
{
    std::ofstream output("data.json");
    output << data;
    output.close();
}

