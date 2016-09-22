/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
 *
 * This file is part of Andama.
 *
 * Andama is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Andama is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#include "mainwindow.h"
#include "engine.h"
#include "qapplication.h"
#include "screenshotprovider.h"
#include <QGuiApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>

#ifndef ANDAMA_SERVICE_MODE //a service/daemon does not use qml related code
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#endif //ANDAMA_SERVICE_MODE

//http://www.qtcentre.org/threads/19534-redirect-qDebug()-to-file

//http://qt-project.org/doc/qt-5/qtglobal.html#qInstallMessageHandler
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }

    /*
    QFile outFile(QCoreApplication::applicationDirPath().append("/log.txt"));
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    */
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool useQML = false;
    if (!useQML)
    {
        qInstallMessageHandler(myMessageOutput);

#ifndef ANDAMA_SERVICE_MODE //a service/daemon does not use widgets related code
        MainWindow w;
        w.show();
#endif //ANDAMA_SERVICE_MODE

        return a.exec();
    }
    else{
        //Load translation file
        /*QTranslator translator;
        translator.load(QLocale::system(), ":/andama","_",QCoreApplication::applicationDirPath(),".qm");
        a.installTranslator(&translator);*/

#ifndef ANDAMA_SERVICE_MODE //a service/daemon does not use qml related code

        ScreenshotProvider screenProvider;
        Engine andamaEngine(&screenProvider);

        QQmlApplicationEngine engine;


        qmlRegisterType<clientserver>("engine.andama.com",1,0,"Engine"); //register type to get enum
        engine.rootContext()->setContextProperty("engine", &andamaEngine);
        engine.addImageProvider(QLatin1String("remotescreen"), &screenProvider);
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        andamaEngine.start();
#endif //ANDAMA_SERVICE_MODE

        return a.exec();
    }
}
