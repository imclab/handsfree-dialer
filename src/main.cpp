/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "dialercontext.h"
#include "dialerapplication.h"
#include "qmlmainwindow.h"
#include "common.h"

#include <QtGui>
#include <QApplication>
#include <QDeclarativeView>
#include <QFile>

int main(int argc, char *argv[])
{
    TRACE
    DialerApplication app(argc, argv);  

    QMLMainWindow *qmw = QMLMainWindow::instance();
    qmw->tryToShow();

    return app.exec();
}

QString stripLineID(QString lineid)
{
    TRACE
    static QRegExp rx = QRegExp("([^0-9*#])");

    if (lineid.indexOf('+') == 0) {
        lineid.replace(rx, "");
        return lineid.insert(0,"+");
    }
    else
        return lineid.replace(rx, "");
}

