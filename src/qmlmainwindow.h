/*
 * dialer - Declarative Dialer UX Main Window.
 * Copyright (c) 2011, Tom Swindell.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef QMLMAINWINDOW_H
#define QMLMAINWINDOW_H

#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include "qmldialer.h"
#include "callitem.h"

class QMLMainWindowPrivate;

class QMLMainWindow : public QDeclarativeView 
{
    Q_OBJECT;

public:
    static   QMLMainWindow* instance();
    static   QMLMainWindow* instanceP(QWidget* parent);
    ~QMLMainWindow();

public Q_SLOTS:
    void hide();
    void tryToShow();
    
    void setAdapter(QMLDialer *adapter); //TODO: Refactor out

protected Q_SLOTS:
    void setupUi();

    void onGeometryChanged();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    explicit QMLMainWindow(QWidget *parent = 0);
    QMLMainWindowPrivate *d;
    QDeclarativeView qv;
    Q_DISABLE_COPY(QMLMainWindow)
};

#endif // QMLMAINWINDOW_H

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
