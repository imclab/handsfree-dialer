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
    TRACE;
    QDBusConnection bus = QDBusConnection::systemBus();
    QStringList serviceNames = bus.interface()->registeredServiceNames();
 	
    if (serviceNames.contains("com.hfdialer"))
    {
        QDBusMessage message = QDBusMessage::createMethodCall("com.hfdialer","/com/dialer","com.hfdialer", "raise");
	bus.call(message,QDBus::NoBlock);
	
	return 0;
    }

    DialerApplication app(argc, argv);  

    QMLMainWindow *qmw = QMLMainWindow::instance();
    
    QString argString(argv[1]); 

    if (argString != "noshow")
       qmw->tryToShow();

    return app.exec();
}

QString stripLineID(QString lineid)
{
    TRACE;
    static QRegExp rx = QRegExp("([^0-9*#])");

    if (lineid.indexOf('+') == 0) {
        lineid.replace(rx, "");
        return lineid.insert(0,"+");
    } else {
        return lineid.replace(rx, "");
    }
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
