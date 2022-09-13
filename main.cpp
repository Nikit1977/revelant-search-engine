#include <QApplication>
#include <QPushButton>

#include "ConverterJSON.h"

#include <iostream>


int main(int argc, char *argv[]) {

    ConverterJSON files_pack;
    ///анализ файла config.json

    //try {
        files_pack.testConfigFile();
    /*}
    catch (std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        std::exit(3);
    }*/


/*stream_parser p;
error_code ec;
p.reset();
p.write( "[1, 2 ", ec );
if( ! ec )
    p.write( ", 3]", ec );
if( ! ec )
    p.finish( ec );
if( ec )
    return;
value jv = p.release();*/


    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
