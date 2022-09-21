
#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H



#include <string>
#include <vector>

#include <boost/json.hpp>

class ConverterJSON {
public:
    ConverterJSON() = default;

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit();

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>
                    answers);
    /**
     * Проверка наличия файла config.son, и ключа config в нем
     */
    void testConfigFile();
private:


    bool testRequestsFile();

    bool getConfigData(boost::json::object source);

    const char* config_file = "..\\config.json";
    const char* requests_file = "..\\requests.json";
    const char* answers_file = "..\\answers.json";
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
