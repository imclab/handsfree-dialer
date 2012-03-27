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

#define CONFIG_KEY_TARGET_UX "/apps/dialer/ux"

#if !defined(CONFIG_DEFAULT_TARGET_UX)
#  define CONFIG_DEFAULT_TARGET_UX "tizen-ux-components"
#endif

 void myMessageOutput(QtMsgType type, const char *msg)
 {
     QFile debugFile("/home/tizen/dialerout.txt");
	debugFile.open(QIODevice::WriteOnly | QIODevice::Append);
     QTextStream out(&debugFile);

	out << msg << "\n";
	debugFile.close();
 }

int main(int argc, char *argv[])
{
  TRACE
   qInstallMsgHandler(myMessageOutput);
   DialerApplication app(argc, argv);  

    QMLMainWindow *qmw = QMLMainWindow::instance();
    //setResizeMode(QDeclarativeView::SizeRootObjectToView);
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

bool currentPageIs(int pagenum)
{
    DialerApplication *app = DialerApplication::instance();
     return true;
}

// Returns a valid QDateTime if parsable as such, otherwise the result
// will be !isValid()
QDateTime qDateTimeFromOfono(const QString &val)
{
    TRACE
    QDateTime result;

    if (val.isEmpty())
        return result;

    // NOTE: Ofono formats time to string with the following format spec:
    //       %Y-%m-%dT%H:%M:%S%z (for example: "2001-10-19T10:32:30-05:00")

    // Start by trying to parse this as an ISODate "YYYY-MM-DDTHH:MM:SSTZD"
    result = QDateTime::fromString(val,Qt::ISODate);
#ifdef WANT_DEBUG
    qDebug() << QString("Converted %1 with Qt::ISODate: %2")
                       .arg(val)
                       .arg(result.toString());
#endif

    if (!result.isValid()) {
    // ISODate conversion has failed, fallback to manual method
    // NOTE: QDateTime::fromString(val, Qt::ISODate) Fails since the date
    //       format from Ofono is in RFC 822 form, but QDateTime can't parse it
        struct tm time_tm;
        QByteArray  bytes = val.toAscii();
        const char *str = bytes.constData();
        if (strptime(str, "%Y-%m-%dT%H:%M:%S%z", &time_tm) != NULL) {
            time_t t = mktime(&time_tm);
            if (t >= (time_t)(0)) {
                result.setTime_t(t);
#ifdef WANT_DEBUG
                qDebug() << QString("Converted %1 with strptime: %2")
                                   .arg(val)
                                   .arg(result.toString());
#endif
            }
        }

        if (!result.isValid())
            qCritical() << QString("Format error, unknown date/time: %1")
                                  .arg(str);
    }

    return result;
}
