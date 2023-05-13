#include "mainwindow.h"
#include "ui_engine.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

#include "moc_mainwindow.cpp"