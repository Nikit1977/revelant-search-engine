#include "InvertedIndex.h"
#include "TextIndexingTask.h"

#include <utility>
#include <iostream>
#include <regex>

#include <QThreadPool>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_texts) {

    if (input_texts.empty()) {
        std::cout << "WARNING! The are no files in dataBase." << std::endl;
    } else {
        freq_dictionary.clear();

        std::size_t dictionaries_count = input_texts.size();
        std::vector<std::map<std::string, std::size_t>> dict_pack(dictionaries_count); //collection of single dictionaries

        for (std::size_t i = 0; i < dictionaries_count; i++) {

            auto singleTask = new TextIndexingTask(std::ref(input_texts[i]), std::ref(dict_pack[i]));
            QThreadPool::globalInstance()->start(singleTask);//либо запустит, либо поместит в очередь задач
        }

        //костыль: ожидание завершения потоков
        while(QThreadPool::globalInstance()->activeThreadCount()) {
        }

        //connect(..., SIGNAL(result(QString)), SLOT(on_result(QString)));

        //объединения отдельных словарей в общий

        for (std::size_t i = 0; i < dictionaries_count; i++) {
            auto it = dict_pack[i].begin();

            while (it != dict_pack[i].end()) {
                Entry value {i, it->second } ;
                freq_dictionary[it->first].push_back(value);
                ++it;
            }
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