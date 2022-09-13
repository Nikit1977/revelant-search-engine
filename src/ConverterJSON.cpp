#include "ConverterJSON.h"
#include <boost/json/src.hpp>
#include <stdexcept>
#include <fstream>


std::vector<std::string> ConverterJSON::GetTextDocuments() {

    return std::vector<std::string>();
}

bool ConverterJSON::getConfigData() {
return false;
}

void ConverterJSON::testConfigFile() {
    std::ifstream file(config_file, std::ios_base::in);
    if (file.is_open()) {
        if (!getConfigData()) throw std::logic_error("Config file is empty");

        file.close();
    } else throw std::runtime_error("Config file is missing");

}