#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connect(ui->actionRestart, SIGNAL(triggered()), ui->centralwidget, SLOT(restart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
