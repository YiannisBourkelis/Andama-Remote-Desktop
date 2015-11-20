#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "cs_thread.h"
#include "qdebug.h"
#include "screenshotworker.h"
#include "qbytearray.h"
#include "qpixmap.h"

cs_thread cst;
Ui::MainWindow w;
screenshotWorker *screenw;

//forward declarations
void myf(char a[]);
void mymessageRecieved(int msgType, char data[],int length);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
