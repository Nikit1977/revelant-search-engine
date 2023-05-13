#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "MainWindow.h"
#include <QApplication>

#include <iostream>
#include <chrono>

#include "version.h"

int main(int argc, char *argv[]) {

    //analyse source files
    ConverterJSON files_pack;

    try {
        files_pack.testConfigFile();
    }

    catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    QApplication application(argc, argv);

    //UI interface
    MainWindow qWindow(nullptr);
    qWindow.show();

    //create freq_dictionary
    InvertedIndex dataBase;
    auto start = std::chrono::steady_clock::now();
    dataBase.UpdateDocumentBase(files_pack.GetTextDocuments());
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
    std::cout << "duration " << elapsed_seconds.count() << "ms\n";

    //prepare search objects
    SearchServer searchServer(dataBase);
    searchServer.setResponsesLimit(files_pack.GetResponsesLimit());
    auto relevanceResult = searchServer.search(files_pack.GetRequests());

    //save result
    files_pack.putAnswers(SearchServer::to_format(relevanceResult));

    return QApplication::exec();
}
