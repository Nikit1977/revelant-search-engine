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
        //для тестов отключить многопоточность. При тестировании в многопоточном режиме
        // не всегда получается получить состоянние ASSERT,
        // т.к. во frec_dictionary std::map<std::string, std::vector<Entry>>, значения vector<Entry> записываюся
        //в произвольном порядке по мере выполнения работы потоков, а не последовательно
#ifdef SINGLE_THREAD
        QThreadPool::globalInstance()->setMaxThreadCount(1);
#endif
        for (std::size_t i = 0; i < input_texts.size(); i++) { //15ms for single-thread//7ms for multi-thread
                                                             //UPD: в релизной версии прироста нет
            auto singleTask = new TextIndexingTask(std::ref(input_texts[i]), std::ref(freq_dictionary), i);
            QThreadPool::globalInstance()->start(singleTask);//либо запустит, либо поместит в очередь задач
        }

        //костыль: ожидание завершения потоков
        while(QThreadPool::globalInstance()->activeThreadCount()) {
        }

        //connect(..., SIGNAL(result(QString)), SLOT(on_result(QString)));
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    } else
        return {};
}