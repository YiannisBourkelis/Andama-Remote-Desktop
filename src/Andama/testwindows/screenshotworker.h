#ifndef SCREENSHOTWORKER_H
#define SCREENSHOTWORKER_H

#include <QPixmap>
#include <QWidget>
#include <QApplication>
#include "QDesktopWidget.h"
#include "qbuffer.h"
#include "mainwindow.h"
#include <QLabel>
#include "cs_thread.h"

class screenshotWorker
{
public:
    screenshotWorker();
    void Starts(QLabel *g,cs_thread *cst);

private:
     QPixmap originalPixmap;
};

#endif // SCREENSHOTWORKER_H
