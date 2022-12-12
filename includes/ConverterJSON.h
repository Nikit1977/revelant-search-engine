
#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <string>
#include <vector>

#include <boost/json.hpp>
/**
 * Класс с предустановленными названиями исходных и результирующих файлов и путями к ним
 * (по умолчанию расположенных в корневой папке проекта),
 * предоставляющий методы для обработки данных файлов (чтение, проверка и запись результатов)
 */
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
* @return возвращает число запросов, указанное в config.json (по умолчанию - 5)
*/
    int GetResponsesLimit();

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

    /**
     * Читает файл формата JSON
     * @param name имя файла
     * @return объект в формате JSON
     * @Exceptions  FileMissEx (отсутствие  файла), FormatEx (не JSON формат)
     */
    boost::json::object readFileJSON(const char* name);
    /**
     * Формирует список значений по определенному ключу
     * @param key ключ в паре key/value
     * @param source исходный объект, в котором осуществляется поиск
     * @return список значений
     */
    std::vector<std::string> values_from(const char* key, boost::json::object &source);

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> &&answers);

    /**
     * Создание json объекта для последующего вывода
     * @param answers результаты поиска по документам SearchServer::search()
     * @return json object
     */
    boost::json::object createJSONObject(std::vector<std::vector<std::pair<int, float>>> answers);

    /**
     * Вызывает проверочные методы, каждый из которых способен вызвать исключение, не допускающее запуск программы
     * Проверка наличия файла config.json, и параметров, требуемых программе (name, version, path и пр.)
     */
    void testConfigFile();

    /**
     * Вспомогательный метод. Доступ к версии программы
     * @return PROJECT_VERSION from CMakeLists.txt
     */
    const char *getEngineVersion();

    /**
     * Вспомогательный метод. Доступ версии для программы из прочитанного config.json
     * @return value from - ["config/version"]
     */
    const char *getEngineVersionJSON();

    [[nodiscard]] const char* getPathToConfigFile() const;
    [[nodiscard]] const char* getPathToRequestFile() const;
    [[nodiscard]] const char* getPathToAnswersFile() const;

    void setPathToConfigFile(const char* name);
    void setPathToRequestFile(const char* name);
    void setPathToAnswersFile(const char* name);

    /**
     * Доступ к сохраненным данным из config.json
     * @return &configInfo
     */
    boost::json::object &getConfigData ();

    /**
     * чтение файла config.json и запись результатов в переменную configInfo
     * @param source путь к файлу
     */
    void createConfigInfo();

    /**
     * Проверяет наличие пары key - ["/config"] / value.
     * value - любое не пустое значение.
     * При несоответствии ожидаемому вызывает исключение - ConfigEmptyEx
     */
    void checkConfigValid();

    /**
    * проверка соответствия версии программы и config.json
    * @return true при равенстве версий, иначе false
    */
    void checkEngineVersion();

private:
    /**
 * объект для хранения в памяти данных из config.json
 */
    boost::json::object configInfo;

    /**
 * предустановленные пути к файлам
 */
    const char* config_file = "..\\config.json";
    const char* requests_file = "..\\requests.json";
    const char* answers_file = "..\\answers.json";

};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
