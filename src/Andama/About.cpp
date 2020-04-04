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

#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

#if  defined Q_OS_WIN || defined Q_OS_LINUX
    QFont lblAppNameFont = ui->lblAppName->font();
    lblAppNameFont.setPointSize(lblAppNameFont.pointSize() * 0.9);

    QFont lblCopyrightFont = ui->lblCopyright->font();
    lblCopyrightFont.setPointSize(lblCopyrightFont.pointSize() * 0.9);

    ui->lblAppName->setFont(lblAppNameFont);
    ui->lblCopyright->setFont(lblCopyrightFont);
    ui->lblVersion->setFont(lblCopyrightFont);
    //ui->txtLicense->setFont(lblCopyrightFont);
    ui->txtLicense->setHtml(ui->txtLicense->toHtml().replace("12pt","10pt"));
#endif

    ui->lblAppName->setText(QGuiApplication::applicationName());
    QString version(APP_VERSION);
    ui->lblVersion->setText(tr("Version %1").arg(version) + " Alpha");
}

About::~About()
{
    delete ui;
}
