#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include "InvertedIndex.h"
#include <unordered_set>

struct RelativeIndex{
    std::size_t doc_id;
    float rank;

    RelativeIndex() = default;
    RelativeIndex(std::size_t in_doc_id, float in_rank) : doc_id(in_doc_id), rank(in_rank) {}


    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
    /**
     * Устанавливает количество ответов на запрос
     * @param count количество ответов
     */
    void setResponsesLimit(int count);
private:
    InvertedIndex _index;
    int responses = 5;
/**
 * Формирует список уникальных слов в нижнем регистре без знаков пунктуации
 * @param words строка представляющая отдельный поисковый запрос
 * @return набор уникальных слов
 */
    std::unordered_set<std::string> getUniq(const std::string& words);

    /**
     * Возвращает таблицу документов с относительным индексом релевантности.
     * Метод сначала вычисляет абсолютный индекс документа (т.е. общее количество слов в документе из запроса),
     * и выводит его отношение к документу с наибольшим совпадением
     * @param words набор уникальных слов
     * @return контейнер map, где key - id_doc / value - относительный индекс (от 0 до 1)
     */
    std::vector<RelativeIndex> getRelativeRelevance(std::unordered_set<std::string> &words);
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
