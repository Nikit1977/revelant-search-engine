#include "TextIndexingTask.h"
#include <regex>

TextIndexingTask::TextIndexingTask(std::string &in_text, std::map<std::string, std::vector<Entry>> &dictionary,
                                   std::size_t id) : text(in_text), text_id(id), dict(&dictionary) {};

void TextIndexingTask::run() {
    std::map<std::string,Entry> single_dictionary;

    for (char & ch : text) {
        if (ch <= 'Z' && ch >= 'A') ch += 32;
    }
    //выделение любого слова
    std::regex someWord("[a-z|0-9]+");
    Entry value {text_id, 0}; //объект Entry  с указанием текущего id документа

    //итерация по отдельным словам с помощью токена из результата совпадений регулярного выражения
    auto it = std::sregex_token_iterator(text.begin(), text.end(), someWord);
    while (it != std::sregex_token_iterator()) {
        //возвращает ссылку на вставленный или существующий элемент. В любом случае осуществляется инкремент Entry.count
        //(оператор ++ перегружен)
        //отказался от оператора [] (aka [*it++]++), так как нужно инициализировать Entry с определенным значением id,
        //а не 0 по умолчанию
        single_dictionary.insert(std::pair(*it++, value)).first->second++;//вставка с инкрементом или инкремент существующего Entry
    }

    //объединения отдельных словарей в общий
    auto key = single_dictionary.begin();

    while(key != single_dictionary.end()) {
        locker.lock();
        (*dict)[key->first].push_back(key->second);
        locker.unlock();
        key++;
    }
}
