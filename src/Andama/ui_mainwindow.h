/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionD_1;
    QAction *actionD_2;
    QAction *actionD_3;
    QWidget *centralWidget;
    QWidget *mainWidget;
    QWidget *connectWidget;
    QLabel *lblLocalHead;
    QLabel *lblRemoteHead;
    QLabel *lblLocalID;
    QLabel *lblLocalPassword;
    QLabel *lblRemotePassword;
    QLabel *lblRemoteID;
    QLabel *lblID;
    QLineEdit *txtRemotePCID;
    QPushButton *btnConnectToRemoteClient;
    QLineEdit *txtLocalPassword;
    QLineEdit *txtRemotePassword;
    QLabel *lblLocalDesc;
    QLabel *lblRemoteDesc;
    QLabel *lblRemoteIDError;
    QProgressBar *remoteConnectProgressBar;
    QLabel *lblRemotePasswordError;
    QWidget *widgetStatus;
    QLabel *lblStatus;
    QWidget *widget_3;
    QLabel *lblDesktop;
    QMenuBar *menuBar;
    QMenu *menuAndama;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(728, 380);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(728, 380));
        MainWindow->setMaximumSize(QSize(728, 380));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionD_1 = new QAction(MainWindow);
        actionD_1->setObjectName(QStringLiteral("actionD_1"));
        actionD_2 = new QAction(MainWindow);
        actionD_2->setObjectName(QStringLiteral("actionD_2"));
        actionD_3 = new QAction(MainWindow);
        actionD_3->setObjectName(QStringLiteral("actionD_3"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        mainWidget = new QWidget(centralWidget);
        mainWidget->setObjectName(QStringLiteral("mainWidget"));
        mainWidget->setGeometry(QRect(0, 0, 728, 380));
        sizePolicy.setHeightForWidth(mainWidget->sizePolicy().hasHeightForWidth());
        mainWidget->setSizePolicy(sizePolicy);
        connectWidget = new QWidget(mainWidget);
        connectWidget->setObjectName(QStringLiteral("connectWidget"));
        connectWidget->setGeometry(QRect(0, 0, 728, 265));
        sizePolicy.setHeightForWidth(connectWidget->sizePolicy().hasHeightForWidth());
        connectWidget->setSizePolicy(sizePolicy);
        connectWidget->setMinimumSize(QSize(728, 265));
        connectWidget->setMaximumSize(QSize(728, 265));
        connectWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connectWidget->setStyleSheet(QStringLiteral("background-image: url(:/images/images/gradient_main.png)"));
        lblLocalHead = new QLabel(connectWidget);
        lblLocalHead->setObjectName(QStringLiteral("lblLocalHead"));
        lblLocalHead->setGeometry(QRect(30, 20, 321, 31));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(18);
        lblLocalHead->setFont(font);
        lblLocalHead->setStyleSheet(QStringLiteral("color: white;"));
        lblRemoteHead = new QLabel(connectWidget);
        lblRemoteHead->setObjectName(QStringLiteral("lblRemoteHead"));
        lblRemoteHead->setGeometry(QRect(390, 20, 321, 31));
        lblRemoteHead->setFont(font);
        lblRemoteHead->setStyleSheet(QStringLiteral("color: white;"));
        lblLocalID = new QLabel(connectWidget);
        lblLocalID->setObjectName(QStringLiteral("lblLocalID"));
        lblLocalID->setGeometry(QRect(10, 119, 111, 31));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        lblLocalID->setFont(font1);
        lblLocalID->setStyleSheet(QStringLiteral("color: white;"));
        lblLocalID->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblLocalPassword = new QLabel(connectWidget);
        lblLocalPassword->setObjectName(QStringLiteral("lblLocalPassword"));
        lblLocalPassword->setGeometry(QRect(10, 163, 111, 31));
        lblLocalPassword->setFont(font1);
        lblLocalPassword->setStyleSheet(QStringLiteral("color: white;"));
        lblLocalPassword->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblRemotePassword = new QLabel(connectWidget);
        lblRemotePassword->setObjectName(QStringLiteral("lblRemotePassword"));
        lblRemotePassword->setEnabled(false);
        lblRemotePassword->setGeometry(QRect(380, 163, 111, 31));
        lblRemotePassword->setFont(font1);
        lblRemotePassword->setStyleSheet(QStringLiteral("color: white;"));
        lblRemotePassword->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblRemoteID = new QLabel(connectWidget);
        lblRemoteID->setObjectName(QStringLiteral("lblRemoteID"));
        lblRemoteID->setEnabled(false);
        lblRemoteID->setGeometry(QRect(380, 119, 111, 31));
        lblRemoteID->setFont(font1);
        lblRemoteID->setStyleSheet(QStringLiteral("color: white;"));
        lblRemoteID->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblID = new QLabel(connectWidget);
        lblID->setObjectName(QStringLiteral("lblID"));
        lblID->setGeometry(QRect(130, 120, 191, 31));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(18);
        font2.setBold(false);
        font2.setWeight(50);
        lblID->setFont(font2);
        lblID->setStyleSheet(QLatin1String("background-color: white;\n"
"border: 1px solid rgb(79, 255, 91);\n"
"color: white;"));
        lblID->setAlignment(Qt::AlignCenter);
        lblID->setMargin(0);
        lblID->setIndent(0);
        txtRemotePCID = new QLineEdit(connectWidget);
        txtRemotePCID->setObjectName(QStringLiteral("txtRemotePCID"));
        txtRemotePCID->setEnabled(true);
        txtRemotePCID->setGeometry(QRect(500, 120, 191, 31));
        txtRemotePCID->setFont(font);
        txtRemotePCID->setStyleSheet(QLatin1String("border: 1px solid rgb(79, 255, 91);\n"
"color: white;"));
        txtRemotePCID->setAlignment(Qt::AlignCenter);
        txtRemotePCID->setClearButtonEnabled(false);
        btnConnectToRemoteClient = new QPushButton(connectWidget);
        btnConnectToRemoteClient->setObjectName(QStringLiteral("btnConnectToRemoteClient"));
        btnConnectToRemoteClient->setEnabled(false);
        btnConnectToRemoteClient->setGeometry(QRect(500, 213, 191, 31));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnConnectToRemoteClient->sizePolicy().hasHeightForWidth());
        btnConnectToRemoteClient->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setWeight(75);
        btnConnectToRemoteClient->setFont(font3);
        btnConnectToRemoteClient->setContextMenuPolicy(Qt::DefaultContextMenu);
        btnConnectToRemoteClient->setStyleSheet(QStringLiteral("color: white"));
        btnConnectToRemoteClient->setAutoDefault(false);
        btnConnectToRemoteClient->setDefault(false);
        txtLocalPassword = new QLineEdit(connectWidget);
        txtLocalPassword->setObjectName(QStringLiteral("txtLocalPassword"));
        txtLocalPassword->setGeometry(QRect(130, 165, 191, 31));
        txtLocalPassword->setFont(font);
        txtLocalPassword->setStyleSheet(QLatin1String("border: 1px solid rgb(79, 255, 91);\n"
"color: white;"));
        txtLocalPassword->setAlignment(Qt::AlignCenter);
        txtLocalPassword->setReadOnly(true);
        txtLocalPassword->setClearButtonEnabled(false);
        txtRemotePassword = new QLineEdit(connectWidget);
        txtRemotePassword->setObjectName(QStringLiteral("txtRemotePassword"));
        txtRemotePassword->setEnabled(true);
        txtRemotePassword->setGeometry(QRect(500, 165, 191, 31));
        txtRemotePassword->setFont(font);
        txtRemotePassword->setStyleSheet(QLatin1String("border: 1px solid rgb(79, 255, 91);\n"
"color: white;"));
        txtRemotePassword->setAlignment(Qt::AlignCenter);
        txtRemotePassword->setClearButtonEnabled(false);
        lblLocalDesc = new QLabel(connectWidget);
        lblLocalDesc->setObjectName(QStringLiteral("lblLocalDesc"));
        lblLocalDesc->setGeometry(QRect(30, 65, 291, 51));
        QFont font4;
        font4.setFamily(QStringLiteral("Arial"));
        font4.setPointSize(12);
        lblLocalDesc->setFont(font4);
        lblLocalDesc->setStyleSheet(QStringLiteral("color: rgb(137, 137, 137)"));
        lblLocalDesc->setTextFormat(Qt::PlainText);
        lblLocalDesc->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblLocalDesc->setWordWrap(true);
        lblRemoteDesc = new QLabel(connectWidget);
        lblRemoteDesc->setObjectName(QStringLiteral("lblRemoteDesc"));
        lblRemoteDesc->setGeometry(QRect(390, 65, 291, 41));
        lblRemoteDesc->setFont(font4);
        lblRemoteDesc->setStyleSheet(QStringLiteral("color: rgb(137, 137, 137)"));
        lblRemoteDesc->setTextFormat(Qt::PlainText);
        lblRemoteDesc->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblRemoteDesc->setWordWrap(true);
        lblRemoteIDError = new QLabel(connectWidget);
        lblRemoteIDError->setObjectName(QStringLiteral("lblRemoteIDError"));
        lblRemoteIDError->setGeometry(QRect(500, 102, 221, 16));
        lblRemoteIDError->setFont(font4);
        lblRemoteIDError->setStyleSheet(QStringLiteral("color: rgb(255, 0, 35)"));
        remoteConnectProgressBar = new QProgressBar(connectWidget);
        remoteConnectProgressBar->setObjectName(QStringLiteral("remoteConnectProgressBar"));
        remoteConnectProgressBar->setEnabled(true);
        remoteConnectProgressBar->setGeometry(QRect(500, 220, 191, 23));
        remoteConnectProgressBar->setMaximum(0);
        remoteConnectProgressBar->setValue(0);
        remoteConnectProgressBar->setTextVisible(false);
        lblRemotePasswordError = new QLabel(connectWidget);
        lblRemotePasswordError->setObjectName(QStringLiteral("lblRemotePasswordError"));
        lblRemotePasswordError->setGeometry(QRect(500, 196, 221, 16));
        lblRemotePasswordError->setFont(font4);
        lblRemotePasswordError->setStyleSheet(QStringLiteral("color: rgb(255, 0, 35)"));
        lblRemotePasswordError->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblLocalHead->raise();
        lblRemoteHead->raise();
        lblLocalID->raise();
        lblLocalPassword->raise();
        lblRemotePassword->raise();
        lblRemoteID->raise();
        lblID->raise();
        txtRemotePCID->raise();
        txtLocalPassword->raise();
        txtRemotePassword->raise();
        lblLocalDesc->raise();
        lblRemoteDesc->raise();
        lblRemoteIDError->raise();
        remoteConnectProgressBar->raise();
        btnConnectToRemoteClient->raise();
        lblRemotePasswordError->raise();
        widgetStatus = new QWidget(mainWidget);
        widgetStatus->setObjectName(QStringLiteral("widgetStatus"));
        widgetStatus->setGeometry(QRect(0, 265, 728, 25));
        widgetStatus->setStyleSheet(QStringLiteral("background-image: url(:/images/images/status_red.png)"));
        lblStatus = new QLabel(widgetStatus);
        lblStatus->setObjectName(QStringLiteral("lblStatus"));
        lblStatus->setGeometry(QRect(0, 0, 728, 25));
        sizePolicy.setHeightForWidth(lblStatus->sizePolicy().hasHeightForWidth());
        lblStatus->setSizePolicy(sizePolicy);
        lblStatus->setFont(font1);
        lblStatus->setStyleSheet(QStringLiteral("color: white;"));
        lblStatus->setAlignment(Qt::AlignCenter);
        widget_3 = new QWidget(mainWidget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setGeometry(QRect(0, 290, 728, 90));
        widget_3->setStyleSheet(QStringLiteral("background-image: url(:/images/images/advertise_here.png)"));
        lblDesktop = new QLabel(centralWidget);
        lblDesktop->setObjectName(QStringLiteral("lblDesktop"));
        lblDesktop->setGeometry(QRect(420, 0, 61, 16));
        lblDesktop->setFocusPolicy(Qt::StrongFocus);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 728, 22));
        menuAndama = new QMenu(menuBar);
        menuAndama->setObjectName(QStringLiteral("menuAndama"));
        MainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(txtRemotePCID, txtRemotePassword);
        QWidget::setTabOrder(txtRemotePassword, btnConnectToRemoteClient);
        QWidget::setTabOrder(btnConnectToRemoteClient, txtLocalPassword);

        menuBar->addAction(menuAndama->menuAction());
        menuAndama->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Andama", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionD_1->setText(QApplication::translate("MainWindow", "d-1", 0));
        actionD_2->setText(QApplication::translate("MainWindow", "d-2", 0));
        actionD_3->setText(QApplication::translate("MainWindow", "d-3", 0));
        lblLocalHead->setText(QApplication::translate("MainWindow", "Local Computer", 0));
        lblRemoteHead->setText(QApplication::translate("MainWindow", "Remote Computer", 0));
        lblLocalID->setText(QApplication::translate("MainWindow", "Your ID:", 0));
        lblLocalPassword->setText(QApplication::translate("MainWindow", "Password:", 0));
        lblRemotePassword->setText(QApplication::translate("MainWindow", "Password:", 0));
        lblRemoteID->setText(QApplication::translate("MainWindow", "Remote ID:", 0));
        lblID->setText(QApplication::translate("MainWindow", "-", 0));
        btnConnectToRemoteClient->setText(QApplication::translate("MainWindow", "Connect to computer", 0));
        txtLocalPassword->setText(QApplication::translate("MainWindow", "-", 0));
        lblLocalDesc->setText(QApplication::translate("MainWindow", "With the following ID and Password you can remotely connect to this computer.", 0));
        lblRemoteDesc->setText(QApplication::translate("MainWindow", "Enter the ID and Password of the remote computer you want to connect.", 0));
        lblRemoteIDError->setText(QString());
        lblRemotePasswordError->setText(QString());
        lblStatus->setText(QApplication::translate("MainWindow", "Connecting...", 0));
        lblDesktop->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        menuAndama->setTitle(QApplication::translate("MainWindow", "Andama", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
