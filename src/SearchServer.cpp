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
    //если нет документов (таблица пуста), то выход из метода
    if (relevance_table.empty()) return {};

    //перевод информации в представление, согласно ТЗ
    std::vector<RelativeIndex> result;
    result.reserve(relevance_table.size());

    //конвертация map to vector
    for (auto &it: relevance_table) {
        result.emplace_back(it.first, it.second);
    }

    //сверка с требуемым числом ответов
    if (result.size() < responses) setResponsesLimit(result.size());

    //частичная сортировка
    std::partial_sort(result.begin(), result.begin() + responses, result.end(),
                      [](const RelativeIndex &lhs, const RelativeIndex &rhs) {
                          if (lhs.rank > rhs.rank) return true; //обратный порядок
                          if (!(lhs.rank < rhs.rank)) return (lhs.doc_id < rhs.doc_id);
                          return false;
                      });

    //модификация (приведение к относительным индексам)

    float maxR_abs = result.begin()->rank;//уже была проверка, что результат не пустой, и 0 быть не может
    for (auto it = result.begin(); it != result.begin() + responses; it++) {
        it->rank /= maxR_abs;
    }

    return {result.begin(), result.begin() + responses};
}

std::vector<std::vector<std::pair<int, float>>>
SearchServer::to_format(std::vector<std::vector<RelativeIndex>> &source) {

    std::vector<std::vector<std::pair<int, float>>> result;
    for (auto & vec : source) {
        std::vector<std::pair<int, float>> tmp;
        for (auto & val : vec) {
            tmp.emplace_back(val.format());
        }
        result.push_back(tmp);
    }
    return result;
}

void SearchServer::setResponsesLimit(std::size_t count) {
    responses = count;
}
