#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cs_thread.h"
#include "qdebug.h"
#include "screenshotworker.h"
#include "qbytearray.h"
#include "qpixmap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
