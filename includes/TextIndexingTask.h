#ifndef SEARCH_ENGINE_TEXTINDEXINGTASK_H
#define SEARCH_ENGINE_TEXTINDEXINGTASK_H
#include "InvertedIndex.h"

#include <QRunnable>


class TextIndexingTask : public QRunnable {

public:
TextIndexingTask() = default;
TextIndexingTask(std::string &in_text, std::map<std::string, std::size_t> &out_dict);

void run() override;

private:

    std::string *text;
    std::map<std::string, std::size_t> *single_dictionary;
};


#endif //SEARCH_ENGINE_TEXTINDEXINGTASK_H
