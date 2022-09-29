#include <QApplication>
#include <QPushButton>

#include "ConverterJSON.h"


#include <iostream>
#include <fstream>

#include "version.h"


///todo сделать сеттер к предустановленным файлам?

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

    auto sourceFiles = files_pack.GetTextDocuments();
    if (sourceFiles.empty()) return -1;

    for (const auto &item : sourceFiles) {
        std::ifstream file(item);

        if (file.is_open()) {
            std::cout << "find file" << std::endl;
            file.close();
        }
    }


    int responsesLimit = files_pack.GetResponsesLimit();

    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
