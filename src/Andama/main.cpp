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
//#include <QMessageBox>
#include <QTranslator> //xreiazetai gia tis metafraseis

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
    case QtInfoMsg:
        txt = QString("Info: %1 (%2:%3, %4)").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
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


/*
 * http://www.itgo.me/a/7231831138432713549/c++-how-to-switch-a-process-between-default-desktop-and-winlogon-desktop
 * https://blogs.msdn.microsoft.com/winsdk/2009/07/14/launching-an-interactive-process-from-windows-service-in-windows-vista-and-later/
 *
void ShowLastErrorMessage(DWORD errCode, LPTSTR errTitle)
{
    LPTSTR errorText = NULL;
    FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&errorText,
    0,
    NULL);
    if ( NULL != errorText )
    {
    WCHAR msg[512] = {0};
    wsprintf(msg, TEXT("%s:\nError Code: %u\n%s\n"), errTitle, errCode, errorText);
    LocalFree(errorText);
    errorText = NULL;
    OutputDebugString(msg);
    }
}



BOOL SetWinSta0Desktop(TCHAR *szDesktopName)
{
    BOOL bSuccess = FALSE;
    HWINSTA hWinSta0 = OpenWindowStation(TEXT("WinSta0"), FALSE, MAXIMUM_ALLOWED);
    if (NULL == hWinSta0) { ShowLastErrorMessage(GetLastError(), TEXT("OpenWindowStation")); }
    bSuccess = SetProcessWindowStation(hWinSta0);
    if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetProcessWindowStation")); }
    HDESK hDesk = OpenDesktop(szDesktopName, 0, FALSE, MAXIMUM_ALLOWED);
    if (NULL == hDesk) { ShowLastErrorMessage(GetLastError(), TEXT("OpenDesktop")); }
    bSuccess = SetThreadDesktop(hDesk);
    if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetThreadDesktop")); }
    if (hDesk != NULL) { CloseDesktop(hDesk); }
    if (hWinSta0 != NULL) { CloseWindowStation(hWinSta0); }
    return bSuccess;
}
*/

//fortwnei to arxeio me tis metafraseis
void loadTranslator(QApplication *a,  QTranslator *translator)
{
    QString lang = QLocale::system().name();
    int lang_code_idx = QLocale::system().name().indexOf("_");
    if (lang_code_idx > -1){
        //krataw mono ti glwssa. p.x. apo el_GR krataw mono to el
        lang.truncate(lang_code_idx);
    }
    QString lang_file = QString(":/Andama_%1.qm").arg(lang);
    translator->load(lang_file);

    a->installTranslator(translator);
}

int main(int argc, char *argv[])
{
    //SetWinSta0Desktop(TEXT("winlogon"));

    QApplication a(argc, argv);

    //fortwsi metafrasis annaloga me ti glwssa tou systimatos
    //Prepei o orismos tis translator na vrisketai sto scope tis main giati alliws den ginetai metafrasi
    QTranslator translator;
    loadTranslator(&a,&translator);

    bool useQML = false;
    if (!useQML)
    {
        qInstallMessageHandler(myMessageOutput);

#ifndef ANDAMA_SERVICE_MODE //a service/daemon does not use widgets related code

        MainWindow w;
        w.show();
#endif //ANDAMA_SERVICE_MODE
        //QString locale = QLocale::system().name();

        //QMessageBox::information(&w,"Language",lang);
        return a.exec();
    }
    else{

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
