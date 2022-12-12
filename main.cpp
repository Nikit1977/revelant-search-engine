#include <QApplication>
#include <QPushButton>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <iostream>

#include "version.h"


int main(int argc, char *argv[]) {

    ConverterJSON files_pack;
    ///анализ файла config.json

    try {
        files_pack.testConfigFile();
    }

    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    InvertedIndex dataBase;
    dataBase.UpdateDocumentBase(files_pack.GetTextDocuments());//создание частотного словаря. Текст исходных документов не сохраняется

    SearchServer searchServer(dataBase); //подготовка объекта поиска
    searchServer.setResponsesLimit(files_pack.GetResponsesLimit());
    auto relevanceResult = searchServer.search(files_pack.GetRequests());

    //сохранение результатов
    files_pack.putAnswers(SearchServer::to_format(relevanceResult));

    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
