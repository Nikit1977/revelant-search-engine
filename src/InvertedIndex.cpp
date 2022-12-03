#include "InvertedIndex.h"
#include <utility>
#include <iostream>
#include <regex>


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    if (docs.empty()) {
        std::cout << "WARNING! The are not \"txt\" files in dataBase." << std::endl;
    } else {
        freq_dictionary.clear();
        for (std::size_t i = 0; i < docs.size(); i++) {
            indexWords(i, docs[i]);
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    } else
        return {};
}

std::size_t InvertedIndex::indexWords(std::size_t id, std::string &source) {
    std::map<std::string,Entry> single_dictionary;

    for (char & ch : source) {
        if (ch <= 'Z' && ch >= 'A') ch += 32;
    }
    std::regex someWord("[a-z|0-9]+");
    Entry value {id, 0}; //объект Entry  с указанием текущего id документа

    //итерация по отдельным словам с помощью токена из результата совпадений регулярного выражения
    auto it = std::sregex_token_iterator(source.begin(), source.end(), someWord);
    while (it != std::sregex_token_iterator()) {
        //возвращает ссылку на вставленный или существующий элемент. В любом случае осуществляется инкремент Entry.count
        //(оператор ++ перегружен)
        //отказался от оператора [] (aka [*it++]++), так как нужно инициализировать Entry с определенным значением id,
        //а не 0 по умолчанию
        single_dictionary.insert(std::make_pair(*it++, value)).first->second++;
    }

    //объединения отдельных словарей в общий
    auto key = single_dictionary.begin();
    while(key != single_dictionary.end()) {
        freq_dictionary[key->first].push_back(key->second);
        key++;
    }

    //печать результата
    auto it2 = freq_dictionary.begin();
    while(it2 != freq_dictionary.end()) {
        std:: cout << it2->first << " ";
        for (int i = 0; i < it2->second.size(); i++) {
            std::cout << it2->second[i].doc_id << " " << it2->second[i].count << " ";
        }
        it2++;
    }
}