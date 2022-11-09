#include <QApplication>
#include <QPushButton>

#include "ConverterJSON.h"


#include <iostream>
#include <fstream>

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

    auto rowTexts = files_pack.GetTextDocuments();
    if (rowTexts.empty()) return -1;

    int responsesLimit = files_pack.GetResponsesLimit();

    auto req = files_pack.GetRequests();

    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
