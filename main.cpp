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

    SearchServer searchServer(dataBase);

    searchServer.setResponsesLimit(files_pack.GetResponsesLimit());
    searchServer.setResponsesLimit(5);
    auto relative = searchServer.search(files_pack.GetRequests());

    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
