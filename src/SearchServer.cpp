#include "SearchServer.h"
#include <algorithm>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {

    std::vector<std::vector<RelativeIndex>> result;
    for (auto &query: queries_input) {
        auto words = getUniq(query); // формирует список уникальных слов
        result.push_back(getRelativeRelevance(words));
    }
    return result;
}

std::unordered_set<std::string> SearchServer::getUniq(const std::string &words) {
    std::string tmp {};
    std::unordered_set<std::string> result;
    for (auto & ch : words) { //     1. Разбивает поисковый запрос на отдельные уникальные слова в нижнем регистре.
        if (ch >= 'a' && ch <= 'z') tmp += ch;
        else if (ch <= 'Z' && ch >= 'A') tmp += char(ch + 32);
        else if (!tmp.empty()) {
            result.insert(tmp);
            tmp.clear();
        }
    }
    if (!tmp.empty()) result.insert(tmp); //чтобы не потерять последнеее слово
    return result;
}

std::vector<RelativeIndex> SearchServer::getRelativeRelevance(std::unordered_set<std::string> &words) {

    //ключ таблицы - id документа; value - количество ключевых слов в документе
    std::map<std::size_t, float> relevance_table;

    //данные о каждом слове считываются из частотного словаря и помещаются в map
    for (auto &word: words) {
        auto entry_list = _index.GetWordCount(word);

        if (!entry_list.empty()) { //если уникальное слово есть в частотном словаре

            //заполнение таблицы
            for (auto &en: entry_list) {
                relevance_table[en.doc_id] += float(en.count);
            }
        }
    }

    //перевод информации в структуру, согласно ТЗ
    std::vector<RelativeIndex> result;
   //result.reserve(relevance_table.size());

    //конвертация map to vector
    for (auto &it: relevance_table) {
        result.emplace_back(it.first, it.second);
    }

    if (result.size() < responses) setResponsesLimit((int)result.size());

    //частичная сортировка
    std::partial_sort(result.begin(), result.begin() + responses, result.end(),
                      [](const RelativeIndex &lhs, const RelativeIndex &rhs) {
                          if (lhs.rank > rhs.rank) return true; //обратный порядок
                          if (!(lhs.rank < rhs.rank)) return (lhs.doc_id < rhs.doc_id);
                      });

    //модификация (приведение к относительным индексам)

    if (!result.empty()) {
        float maxR_abs = result.begin()->rank;
        if (!maxR_abs) return {};
        for (auto it = result.begin(); it != result.begin() + responses; it++) {
            it->rank /= maxR_abs;
        }
    }
    return {result.begin(), result.begin() + responses};
}



void SearchServer::setResponsesLimit(int count) {
    responses = count;
}
