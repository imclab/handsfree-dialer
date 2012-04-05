/*
 * dialer - Declarative Dialer UX Main Window.
 * Copyright (c) 2011, Tom Swindell.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "common.h"
#include "dialerapplication.h"

#include "qmlcallitem.h"
#include "qmldialer.h"

#include "qmlmainwindow.h"

#include <QtDeclarative>

#define CONFIG_KEY_QML_LOAD_URL "/apps/dialer/qml/url"

#define DEFAULT_QML_LOAD_URL "file:///usr/share/hfdialer/qml/main.qml"

class QMLMainWindowPrivate
{
public:
    QMLMainWindowPrivate()
        : adapter(NULL),
          engine(NULL),
          component(NULL),
          item(NULL)
    { TRACE; }

    QMLDialer               *adapter;
    QDeclarativeEngine      *engine;
    QDeclarativeView        *qdv; 
    
    QDeclarativeComponent   *component;
    QDeclarativeItem        *item;
};

static void registerDataTypes()
{
    TRACE;
    qmlRegisterType<QMLDialer>("com.tizen.hfdialer", 1, 0, "Dialer");

    qmlRegisterUncreatableType<QMLCallItem>("com.tizen.hfdialer", 1, 0, "CallItem", "");
}

QMLMainWindow::QMLMainWindow(QWidget *parent)
    : QDeclarativeView(parent),
      d(new QMLMainWindowPrivate)
{
    TRACE;
    DialerApplication *da = DialerApplication::instance();
    CallManager *cm = ManagerProxy::instance()->callManager();

    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    this->setWindowTitle(qtTrId("xx_window"));

    this->setupUi();

    da->setActiveWindow(this);

    connect(this->engine(), SIGNAL(quit()), this, SLOT(close()));
}

QMLMainWindow::~QMLMainWindow()
{
    TRACE;
    delete this->d;
}

QMLMainWindow* QMLMainWindow::instance()
{
    TRACE;
    static QMLMainWindow *_instance = NULL;

    if(_instance == NULL)
        {
            registerDataTypes();
            _instance = new QMLMainWindow;
        }

    return _instance;
}


QMLMainWindow* QMLMainWindow::instanceP(QWidget* parent)
{
    TRACE;
    static QMLMainWindow *_instance = NULL;

    if(_instance == NULL)
        {
            registerDataTypes();
            _instance = new QMLMainWindow(parent);
        }

    return _instance;
}

void QMLMainWindow::setupUi()
{
    TRACE;
    MGConfItem qmlUrl(CONFIG_KEY_QML_LOAD_URL);

    d->engine = new QDeclarativeEngine(this);

    d->engine->addImportPath("/usr/share/hfdialer/qml/base");

    d->engine->rootContext()->setContextProperty("controller", this); //TODO: Remove
    this->setSource(QUrl::fromLocalFile("/usr/share/hfdialer/qml/main.qml"));
    d->component = new QDeclarativeComponent(d->engine, this);
    d->component->loadUrl(qmlUrl.value(DEFAULT_QML_LOAD_URL).toString());

    if(d->component->isError())
        {
            qCritical() << "Failed to load QML Component:" << d->component->errorString();
            return;
        }

    d->item = qobject_cast<QDeclarativeItem*>(d->component->create());
    if(!d->item)
        {
            qCritical() << "Failed to create item from component!";
            return;
        }
}

void QMLMainWindow::tryToShow()
{
    TRACE;
     
    if (d->component->isReady())
        {
            DialerApplication *da = DialerApplication::instance();
            da->setActiveWindow(this);
            da->activeWindow()->show();
            da->activeWindow()->activateWindow();
            da->activeWindow()->raise();
            this->show();
        }
 
}
void QMLMainWindow::hide()
{
    TRACE;
    QGraphicsView::hide();
}

void QMLMainWindow::closeEvent(QCloseEvent *event)
{
    TRACE;
    event->accept();

}

void QMLMainWindow::onGeometryChanged()
{
    TRACE;
}

void QMLMainWindow::setAdapter(QMLDialer *adapter)
{
    TRACE;
    d->adapter = adapter;
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
