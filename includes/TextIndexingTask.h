#ifndef SEARCH_ENGINE_TEXTINDEXINGTASK_H
#define SEARCH_ENGINE_TEXTINDEXINGTASK_H
#include "InvertedIndex.h"

#include <QRunnable>
#include <QMutex>


class TextIndexingTask : public QRunnable {

public:
TextIndexingTask() = default;
TextIndexingTask(std::string &text, std::map<std::string, std::vector<Entry>> &dictionary, std::size_t id);

void run() override;

private:
    QMutex locker;
    std::size_t text_id;
    std::string text;
    std::map<std::string, std::vector<Entry>> *dict;
};


#endif //SEARCH_ENGINE_TEXTINDEXINGTASK_H
