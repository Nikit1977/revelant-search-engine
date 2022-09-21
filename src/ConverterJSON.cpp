#include "ConverterJSON.h"
#include "ConfigErrorExceptions.h"

#include <fstream>
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments() {

    return std::vector<std::string>();
}

bool ConverterJSON::getConfigData(boost::json::object source) {
return false;
}

void ConverterJSON::testConfigFile() {

    std::ifstream file(config_file);
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

        ///если нет ошибок
        if (ec) throw ConfigInvalidEx();

        ///если из прочитанного сложился json формат
        parser.finish(ec);
        if (ec) throw ConfigInvalidEx();

        auto result = parser.release().as_object();
        std::cout << result << std::endl;

        if (auto p = result.if_contains("config")) {
            std::cout << "config found" << std::endl;
            std::cout << *(p->as_object().if_contains("name")) << std::endl;



        }
    } else throw ConfigFileMissEx();
}