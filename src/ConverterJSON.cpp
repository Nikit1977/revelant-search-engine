#include "ConverterJSON.h"
#include "ConfigErrorExceptions.h"
#include "version.h"

#include <fstream>
#include <filesystem>
#include <iostream>


boost::json::object &ConverterJSON::getConfigData() {
return configInfo;
}

void ConverterJSON::testConfigFile() {

    createConfigInfo();
    checkConfigValid();
    checkEngineVersion();
}

void ConverterJSON::createConfigInfo() {

    configInfo = readFileJSON(getPathToConfigFile());
}

boost::json::object ConverterJSON::readFileJSON(const char *name) {

    std::ifstream file(name);
    boost::json::object result;
    if (file.is_open()) {
        boost::json::stream_parser parser;
        boost::json::error_code ec;
        ///читаем из файла кусками по 512 байт
        do {
            char buf[512];
            file.read(buf, sizeof(buf));
            ///добавляем прочитанные куски в парсер
            parser.write(buf, file.gcount(), ec);
        } while (!file.eof());
        file.close();

        if (ec) throw FormatEx(); //если ошибка

        parser.finish(ec);
        if (ec) throw FormatEx(); //если из прочитанного не сложился json формат

        result = parser.release().as_object();

    } else throw FileMissEx();

    return result;
}

void ConverterJSON::checkConfigValid() {
    auto it = getConfigData().find("config");
    if (it == getConfigData().end() || it->value().as_object().empty()) {
        throw ConfigEmptyEx();
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {

    std::vector<std::string> files = values_from("files", getConfigData());

    for (auto &it : files) {
         std::filesystem::path path = it;
         it = path.make_preferred().string();
    }

    return files;
}

std::vector<std::string> ConverterJSON::values_from(const char *key, boost::json::object &source) {

    std::vector<std::string> result;
    if (auto jv = source.if_contains(key)) {

        ///проверка, если ресурсные файлы представлены в виде списка (массива)
        try {
            auto list = jv->as_array(); //если не массив, вызовет исключение
            for (const auto & it : list) {
                result.push_back(boost::json::value_to<std::string>(it) );
            }
        }
            /// или ресурсные файлы представлены в виде одного файла (строки, в том числе пустой)
        catch (std::invalid_argument &ex) {

            if (jv->is_string()) {
                result.push_back(boost::json::value_to<std::string>(*jv));
            }
        }
    }
    return result;
}

std::vector<std::string> ConverterJSON::GetRequests() {

    auto fileData = readFileJSON(getPathToRequestFile());
    return values_from("requests", fileData);
}

void ConverterJSON::checkEngineVersion() {
    if (std::strcmp(getEngineVersion(), getEngineVersionJSON()) != 0) throw ConfigVersionEx();
}

const char* ConverterJSON::getEngineVersion() {
    return PROJECT_VERSION;
}

const char *ConverterJSON::getEngineVersionJSON() {
    return getConfigData().at("config").at("version").as_string().c_str();
}

int ConverterJSON::GetResponsesLimit() {

    const int responses_default = 5;
    int responses = 0;

    try {
        responses = (int)getConfigData().at("config").at("max_responses").as_int64();
    }
    catch (...) {return responses_default;}

    return (responses > 0 ? responses : responses_default);
}

const char *ConverterJSON::getPathToConfigFile() const {
    return config_file;
}

const char *ConverterJSON::getPathToRequestFile() const {
    return requests_file;
}

const char *ConverterJSON::getPathToAnswersFile() const {
    return answers_file;
}
