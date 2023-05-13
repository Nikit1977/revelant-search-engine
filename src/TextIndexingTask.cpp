#include "TextIndexingTask.h"
#include <regex>

TextIndexingTask::TextIndexingTask(std::string &in_text, std::map<std::string, std::size_t> &out_dict)
                                                   : text(&in_text), single_dictionary(&out_dict) {};

void TextIndexingTask::run() {

    //to_lower
    for (auto & ch : *text) {
        if (ch <= 'Z' && ch >= 'A') ch += 32;
    }

    //шаблоны
    std::regex someWord("[a-z|0-9]+");

    //итерация по отдельным словам с помощью токена из результата совпадений регулярного выражения
    auto it = std::sregex_token_iterator(text->begin(), text->end(), someWord);
    while (it != std::sregex_token_iterator()) {
        (*single_dictionary)[*it]++;
        ++it;
    }


}
