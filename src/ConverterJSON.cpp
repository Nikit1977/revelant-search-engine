#include "ConverterJSON.h"
#include "ConfigErrorExceptions.h"
#include "version.h"

#include <fstream>
#include <filesystem>
#include <iostream>

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
    boost::json::stream_parser parser; //парсер

    if (file.is_open()) {
        boost::json::error_code ec;        //поле для хранения возможной ошибки
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

    } else throw FileMissEx();

    return parser.release().as_object();
}

void ConverterJSON::checkConfigValid() {
    auto it = getConfigData().find("config");
    if (it == getConfigData().end() || it->value().as_object().empty()) {
        throw ConfigEmptyEx();
    }
}
//todo: разбить на два метода
std::vector<std::string> ConverterJSON::GetTextDocuments() {

    //получение путей к файлам
    std::vector<std::string> source_files = values_from("files", getConfigData());

    //конвертация путей к файлам в нативный для системы формат и запись их в отдельный список
    for (auto &it : source_files) {
         std::filesystem::path path(it);
         path.make_preferred().string();
         it = path.make_preferred().string();
    }
    //вектор текстов
    std::vector<std::string> row_texts;

    //заполнение вектора текстов текстами из списка файлов
    for (auto const& path : source_files) {

        std::ifstream file(path);
        if (file.is_open()) {

            std::string text;
            text.reserve(std::filesystem::file_size(path));
            text.assign(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());

            file.close();
            row_texts.push_back(text);
        }
    }

    return row_texts;
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
//todo: разобраться с универсальными ссылками
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> &&answers) {
    auto jsonAnswers= createJSONObject(std::move(answers));

    std::ofstream file(getPathToAnswersFile(), std::ios_base::trunc);
    if (file.is_open()) {
        file << jsonAnswers;
        file.close();
    }
}

boost::json::object ConverterJSON::createJSONObject(std::vector<std::vector<std::pair<int, float>>> answers) {
    boost::json::object result_list {{"answers", boost::json::object()}};
    int docNumber { 0 };
    for (auto & answer : answers) {

        boost::json::object result {{"result", true}};

        if (!answer.empty()) {
            boost::json::array relev_list;
            for (auto &relevance: answer) {
                //todo: ограничить точность числа float
                boost::json::value jv = {{"docid", relevance.first}, {"rank", relevance.second}};
                relev_list.emplace_back(jv);
            }
            result.insert({{"relevance", std::move(relev_list)}});

        } else result["result"] = false;

        result_list["answers"].as_object().insert({{"request_" + std::to_string(docNumber++), std::move(result)}});
    }
    return result_list;
}

std::vector<std::string> ConverterJSON::GetRequests() {

    auto fileData = readFileJSON(getPathToRequestFile());
    return values_from("requests", fileData);
}

void ConverterJSON::checkEngineVersion() {
    if (std::strcmp(getEngineVersion(), getEngineVersionJSON()) != 0)
        throw ConfigVersionEx();
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
    //если не числовой  формат
    catch (...) {return responses_default;}

    return (responses > 0 ? responses : responses_default);
}

boost::json::object &ConverterJSON::getConfigData() {
    return configInfo;
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

void ConverterJSON::setPathToConfigFile(const char *name) {
    config_file = name;
}

void ConverterJSON::setPathToRequestFile(const char *name){
    requests_file = name;
}

void ConverterJSON::setPathToAnswersFile(const char *name) {
    answers_file = name;
}
