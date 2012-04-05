/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "common.h"
#include "dbustypes.h"
#include "dialerapplication.h"
#include "dialercontext.h"
#include "dbusdialeradapter.h"
#include "pacontrol.h"
#include "qmlmainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QApplication>

#define OFONO_VOICECALLMANAGER_INTERFACE "org.ofono.VoiceCallManager"

DialerApplication::DialerApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    TRACE;
    init();
}

void DialerApplication::releasePrestart()
{
    TRACE;
    // Now is the time for set up and display of information
    // that needs to be done to allow the dialeror other
    // pages to display correctly when opened. GenericPage has a
    // activateWidgets() method for common setup and
    // each Page type (dialer, people, favorites, etc) can also
    // implement the method for Page specific setup and signal
    // and slot connections

    
}

void DialerApplication::restorePrestart()
{
    TRACE;
    // Now is the time for clean up and resetting an information
    // that needs to be done to allow the dialer pages to display
    // correctly when reopened. GenericPage has a
    // deactivateAndResetWidgets() method for common setup and
    // each Page type (dialer, people, favorites, etc) can also
    // implement the method for Page specific clean up

    // Call the default implementation to hide the window.
}

void DialerApplication::connectAll()
{
    TRACE;

    ManagerProxy *m_manager = ManagerProxy::instance();
    if (m_manager->modem() &&
        m_manager->modem()->isValid() &&
        !m_manager->modem()->path().isEmpty())
        {
            qDebug() << QString("Connecting to CallManager....");
            m_manager->setNetwork(m_manager->modem()->path());
            m_manager->setCallManager(m_manager->modem()->path());
            m_manager->setVolumeManager(m_manager->modem()->path());
            m_manager->setVoicemail(m_manager->modem()->path());

            connect(m_manager->network(), SIGNAL(connected()), this,
                    SLOT(networkConnected()));
            connect(m_manager->network(), SIGNAL(disconnected()), this,
                    SLOT(networkDisconnected()));
            connect(m_manager->callManager(), SIGNAL(connected()), this,
                    SLOT(callManagerConnected()));
            connect(m_manager->callManager(), SIGNAL(disconnected()), this,
                    SLOT(callManagerDisconnected()));
            connect(m_manager->callManager(), SIGNAL(callsChanged()), this,
                    SLOT(onCallsChanged()));
            connect(m_manager->voicemail(), SIGNAL(messagesWaitingChanged()), this,
                    SLOT(messagesWaitingChanged()));
            PAControl* paControl = PAControl::instance();
        qDebug()<<"UBER DEBUG!!!  I can has connect with paControl onCallsChanged";
            connect(m_manager->callManager(), SIGNAL(callsChanged()), paControl,
                    SLOT(onCallsChanged()));
        }
}

bool DialerApplication::isConnected()
{
    TRACE;
    return m_connected;
}

bool DialerApplication::hasError() const
{
    TRACE;
    return !m_lastError.isEmpty();
}

QString DialerApplication::lastError() const
{
    TRACE;
    return m_lastError;
}

void DialerApplication::setError(const QString msg)
{
    TRACE;
    m_lastError.clear();
    m_lastError = QString(msg);
}

DialerApplication *DialerApplication::instance()
{
    TRACE;
    return qobject_cast<DialerApplication *>(QApplication::instance());
}

void DialerApplication::init()
{
    TRACE;
    m_connected = false;
    m_lastError = QString();

    // Notify Qt of our custom DBus MetaTypes
    registerMyDataTypes();

    m_manager = ManagerProxy::instance();
    if (!m_manager || !m_manager->isValid())
        //% "Failed to connect to org.ofono.Manager: is ofonod running?"
        setError(qtTrId("xx_no_ofono_error"));
    else
        m_connected = true;

    DBusDialerAdapter *adapter = new DBusDialerAdapter(this);
    if(!adapter)
        {
            qWarning() << "DBus adapter instantiation failed.";
        }

    if(!QDBusConnection::sessionBus().registerObject(DBUS_SERVICE_PATH, this))
        {
            qCritical() << "Error registering dbus object:" <<
                QDBusConnection::sessionBus().lastError().message();
        }
    connect(m_manager, SIGNAL(modemChanged()),
            SLOT(modemChanged()));

   this->connectAll();

}

void DialerApplication::modemChanged()
{
    TRACE;
    if (m_manager->modem() != NULL)
        {
            connect(m_manager->modem(), SIGNAL(connected()),
                    SLOT(modemConnected()));
            connect(m_manager->modem(), SIGNAL(disconnected()),
                    SLOT(modemDisconnected()));
    }
    else
    {
        qDebug()<<"modem is null";
        }
}

void DialerApplication::modemConnected()
{
    TRACE;
    //TODO: Handle multiple modems
    if (m_manager->modem() && m_manager->modem()->isValid())
        {
            m_modem = m_manager->modem();

            m_modem->setPowered(true);
	
            qDebug() << QString("Modem connected");
            connect(m_modem, SIGNAL(interfacesChanged(QStringList)), this,
                    SLOT(modemInterfacesChanged(QStringList)));
            connect(m_modem, SIGNAL(poweredChanged(bool)), this,
                    SLOT(modemPowered(bool)));

            if (m_modem->powered() &&
                m_modem->interfaces().contains(OFONO_VOICECALLMANAGER_INTERFACE))
                {
                    /* connect all now, modem is enabled */
                    this->connectAll();
                }
        }
}

void DialerApplication::modemDisconnected()
{
    TRACE;
    //TODO: Handle multiple modems
}

void DialerApplication::modemInterfacesChanged(QStringList interfaces)
{
    TRACE;
    qDebug() << QString("Modem Interfaces: ") << interfaces;

    if (interfaces.contains(OFONO_VOICECALLMANAGER_INTERFACE) &&
        m_manager->modem()->powered())
        {
            this->connectAll();
        }
}

void DialerApplication::modemPowered(bool isPowered)
{
    TRACE;
    qDebug() << QString("Modem Powered: ") << isPowered;

    if (isPowered &&
        m_manager->modem()->interfaces().contains(OFONO_VOICECALLMANAGER_INTERFACE))
        {
            this->connectAll();
        }
}

void DialerApplication::networkConnected()
{
    TRACE;
    if (m_manager->network() && m_manager->network()->isValid())
        m_network = m_manager->network();
}

void DialerApplication::networkDisconnected()
{
    TRACE;
}

void DialerApplication::callManagerConnected()
{
    TRACE;
    if (m_manager->callManager() && m_manager->callManager()->isValid())
        m_callManager = m_manager->callManager();


    qDebug() << QString("Disconnect calls changed signal");
    disconnect(m_callManager, SIGNAL(callsChanged()));

    qDebug() << QString("Disconnect incoming signal");
    disconnect(m_callManager, SIGNAL(incomingCall(CallItem*)));

    qDebug() << QString("Disconnect resource lost");
    disconnect(m_callManager, SIGNAL(callResourceLost(const QString)));
}

void DialerApplication::callManagerDisconnected()
{
    TRACE;
    qDebug() << QString("CallMgr disconnected");
}

void DialerApplication::messagesWaitingChanged()
{
    TRACE;
}

void DialerApplication::onCallsChanged()
{
    TRACE;
    QMLMainWindow::instance()->tryToShow();
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
