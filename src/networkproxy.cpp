/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "networkproxy.h"
#include <QDebug>

/* **************************************************************
 * Network Operator Class
 * **************************************************************/

OperatorProxy::OperatorProxy(const QString &operatorPath)
    : org::ofono::NetworkOperator(OFONO_SERVICE,
                                  operatorPath,
                                  QDBusConnection::systemBus()),
      m_path(operatorPath), m_countryCode(""),
      m_networkCode(""), m_name(""), m_status(""), m_technologies("")
{
    if (!isValid()) {
        qCritical() << org::ofono::NetworkOperator::staticInterfaceName() <<
            " connection failed: " << lastError().message();
    } else {
        QDBusPendingReply<QVariantMap> reply;
        QDBusPendingCallWatcher * watcher;

        reply = GetProperties();
        watcher = new QDBusPendingCallWatcher(reply);

        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(operatorDBusGetPropDone(QDBusPendingCallWatcher*)));
    }
}

OperatorProxy::~OperatorProxy()
{
}

QString OperatorProxy::path() const { return m_path; }
QString OperatorProxy::countryCode() const { return m_countryCode; }
QString OperatorProxy::networkCode() const { return m_networkCode; }
QString OperatorProxy::name() const { return m_name; }
QString OperatorProxy::status() const { return m_status; }
QStringList OperatorProxy::technologies() const { return m_technologies; }

void OperatorProxy::operatorDBusGetPropDone(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;

    if (reply.isError()) {
        // TODO: Handle this properly
        qCritical() << org::ofono::NetworkOperator::staticInterfaceName() <<
            ".GetProperties() failed: " << reply.error().message();
    } else {
        QVariantMap properties = reply.value();
        m_countryCode  = qdbus_cast<QString>(properties["MobileCountryCode"]);
        m_networkCode  = qdbus_cast<QString>(properties["MobileNetworkCode"]);
        m_name         = qdbus_cast<QString>(properties["Name"]);
        m_status       = qdbus_cast<QString>(properties["Status"]);
        m_technologies = qdbus_cast<QStringList>(properties["Technologies"]);
    }
}

/* **************************************************************
 * Network Registration Class
 * **************************************************************/
NetworkProxy::NetworkProxy(const QString &modemPath)
    : org::ofono::NetworkRegistration(OFONO_SERVICE,
                                      modemPath,
                                      QDBusConnection::systemBus()),
      m_mode(""), m_name(""), m_status(""), m_connected(false)
{
    if (!isValid()) {
        qCritical() << org::ofono::NetworkRegistration::staticInterfaceName() <<
            " connection failed: " << lastError().message();
    } else {
        QDBusPendingReply<QVariantMap> reply;
        QDBusPendingCallWatcher * watcher;

        reply = GetProperties();
        watcher = new QDBusPendingCallWatcher(reply);

        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(networkDBusGetPropDone(QDBusPendingCallWatcher*)));
    }
}

NetworkProxy::~NetworkProxy()
{
}

QList<OperatorProxy*> NetworkProxy::operators() const { return m_operators; }
OperatorProxy* NetworkProxy::currentOperator() const
{
    return m_currentOperator;
}
QString NetworkProxy::mode() const { return m_mode; }
QString NetworkProxy::name() const { return m_name; }
QString NetworkProxy::status() const { return m_status; }

void NetworkProxy::networkDBusGetPropDone(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;

    if (reply.isError()) {
        // TODO: Handle this properly
        qCritical() << org::ofono::NetworkRegistration::staticInterfaceName() <<
            ".GetProperties() failed: " << reply.error().message();
    } else {
        QVariantMap properties = reply.value();
        QList<QDBusObjectPath> paths =
            qdbus_cast<QList<QDBusObjectPath> >(properties["AvailableOperators"]);

        foreach (QDBusObjectPath p, paths) {
            QString path = QString(p.path());
            OperatorProxy *op = new OperatorProxy(path);

            m_operatorPaths.append(path);
            m_operators.append(op);

        }
        m_mode   = qdbus_cast<QString>(properties["Mode"]);
        m_name   = qdbus_cast<QString>(properties["Operator"]);
        m_status = qdbus_cast<QString>(properties["Status"]);

        // First sucessfull GetProperties == connected
        if (!m_connected) {
            m_connected = true;
            emit connected();
        }
    }
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
