/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *lblAppName;
    QLabel *lblVersion;
    QLabel *lblCopyright;
    QTextEdit *txtLicense;
    QHBoxLayout *horizontalLayout;
    QLabel *lblSupport;
    QLabel *lblSourceCode;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QStringLiteral("About"));
        About->resize(550, 380);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(About->sizePolicy().hasHeightForWidth());
        About->setSizePolicy(sizePolicy);
        About->setMinimumSize(QSize(550, 380));
        About->setMaximumSize(QSize(550, 380));
        About->setStyleSheet(QStringLiteral("background-color: rgb(10,45,58)"));
        verticalLayout_2 = new QVBoxLayout(About);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblAppName = new QLabel(About);
        lblAppName->setObjectName(QStringLiteral("lblAppName"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(17);
        font.setBold(false);
        font.setWeight(50);
        lblAppName->setFont(font);
        lblAppName->setStyleSheet(QStringLiteral("color: white;"));
        lblAppName->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblAppName);

        lblVersion = new QLabel(About);
        lblVersion->setObjectName(QStringLiteral("lblVersion"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        lblVersion->setFont(font1);
        lblVersion->setStyleSheet(QStringLiteral("color: white;"));
        lblVersion->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblVersion);

        lblCopyright = new QLabel(About);
        lblCopyright->setObjectName(QStringLiteral("lblCopyright"));
        lblCopyright->setFont(font1);
        lblCopyright->setStyleSheet(QStringLiteral("color: white;"));
        lblCopyright->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblCopyright);

        txtLicense = new QTextEdit(About);
        txtLicense->setObjectName(QStringLiteral("txtLicense"));
        txtLicense->setFont(font1);
        txtLicense->setStyleSheet(QLatin1String("background-color: rgb(13,68,71);\n"
"padding-left:15; \n"
"padding-top:15; \n"
"padding-bottom:15; \n"
"padding-right:15"));
        txtLicense->setLineWidth(1);
        txtLicense->setReadOnly(true);
        txtLicense->setTextInteractionFlags(Qt::TextBrowserInteraction);

        verticalLayout->addWidget(txtLicense);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lblSupport = new QLabel(About);
        lblSupport->setObjectName(QStringLiteral("lblSupport"));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        lblSupport->setFont(font2);
        lblSupport->setAlignment(Qt::AlignCenter);
        lblSupport->setOpenExternalLinks(true);

        horizontalLayout->addWidget(lblSupport);

        lblSourceCode = new QLabel(About);
        lblSourceCode->setObjectName(QStringLiteral("lblSourceCode"));
        lblSourceCode->setFont(font2);
        lblSourceCode->setAlignment(Qt::AlignCenter);
        lblSourceCode->setOpenExternalLinks(true);

        horizontalLayout->addWidget(lblSourceCode);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About", 0));
        lblAppName->setText(QApplication::translate("About", "TextLabel", 0));
        lblVersion->setText(QApplication::translate("About", "TextLabel", 0));
        lblCopyright->setText(QApplication::translate("About", "Copyright (c) 2015 Yiannis Bourkelis", 0));
        txtLicense->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Lucida Grande UI'; color:#ffffff;\">LICENSE</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Lucida Grande UI';\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Lucida Grande UI'; color:#ffffff;\">Andama is free software: you can redistribute it and/or modify it und"
                        "er the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Lucida Grande UI'; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Lucida Grande UI'; color:#ffffff;\">Andama is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Lucida Grande UI'; color:#ffffff;\"><br /></p></"
                        "body></html>", 0));
        lblSupport->setText(QApplication::translate("About", "<a href=\"http://www.andama.org\"><span style=\" text-decoration: underline; color:#0000ff;\">Community</span></a>", 0));
        lblSourceCode->setText(QApplication::translate("About", "<a href=\"https://github.com/YiannisBourkelis/Andama\"><span style=\" text-decoration: underline; color:#0000ff;\">Source code</span></a>", 0));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
