#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ui_engine.h"

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
    QApplication app(argc, argv);

    //UI interface
    QMainWindow qMainWindow;
    Ui::MainWindow uiMain;
    uiMain.setupUi(&qMainWindow);
    qMainWindow.show();

    //create freq_dictionary
    InvertedIndex dataBase;
    dataBase.UpdateDocumentBase(files_pack.GetTextDocuments());

    //prepare search objects
    SearchServer searchServer(dataBase);
    searchServer.setResponsesLimit(files_pack.GetResponsesLimit());
    auto relevanceResult = searchServer.search(files_pack.GetRequests());

    //save result
    files_pack.putAnswers(SearchServer::to_format(relevanceResult));

    return QApplication::exec();
}
