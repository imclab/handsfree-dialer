/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QDateTime>
#include "common.h"
#include "callproxy.h"
#include "managerproxy.h"

// Returns a valid QDateTime if parsable as such, otherwise the result
// will be !isValid()
static QDateTime qDateTimeFromOfono(const QString &val)
{
    TRACE;
    QDateTime result;

    if (val.isEmpty())
        return result;

    // NOTE: Ofono formats time to string with the following format spec:
    //       %Y-%m-%dT%H:%M:%S%z (for example: "2001-10-19T10:32:30-05:00")

    // Start by trying to parse this as an ISODate "YYYY-MM-DDTHH:MM:SSTZD"
    result = QDateTime::fromString(val,Qt::ISODate);
#ifdef VERBOSE
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
#ifdef VERBOSE 
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

CallProxy::CallProxy(const QString &callPath)
    : org::ofono::VoiceCall(OFONO_SERVICE,
                            callPath,
                            QDBusConnection::systemBus()),
      m_lineid(QString()),
      m_state(QString()),
      m_startTime(QDateTime()),
      m_reason(QString()),
      m_connected(false),
      m_multiparty(false)
{
    TRACE;

    if (!org::ofono::VoiceCall::isValid()) {
        qCritical() << QString("Failed to connect to %1 for call %2:\n\t%3")
            .arg(staticInterfaceName())
            .arg(callPath)
            .arg(lastError().message());
    } else {
        QDBusPendingReply<QVariantMap> reply;
        QDBusPendingCallWatcher *watcher;

        reply = GetProperties();
        watcher = new QDBusPendingCallWatcher(reply);

        // Force this to be sync to ensure we have initial properties
        watcher->waitForFinished();
        getPropertiesFinished(watcher);

        if (isValid()) {
            connect(this,
                    SIGNAL(PropertyChanged(const QString&,const QDBusVariant&)),
                    SLOT(propertyChanged(const QString&,const QDBusVariant&)));
            connect(this, SIGNAL(DisconnectReason(const QString&)),
                    SLOT(disconnectReason(const QString&)));
        } else {
            qCritical() << QString("Invalid CallProxy instance: state == %1")
                .arg(m_state);
        }
    }
}

CallProxy::~CallProxy()
{
    TRACE;
    // FIXME: Do something here!!!
}

bool CallProxy::isValid()
{
    TRACE;
    return (org::ofono::VoiceCall::isValid() &&
            m_connected &&
            (m_state != "disconnected"));
}

QString CallProxy::lineID() const
{
    TRACE;
    return m_lineid;
}

QString CallProxy::name() const
{
    TRACE;
    return m_name;
}

QString CallProxy::state() const
{
    TRACE;
    return m_state;
}

QDateTime CallProxy::startTime() const
{
    TRACE;
    return m_startTime;
}

int CallProxy::duration() const
{
    TRACE;
    if (m_startTime.isValid())
        return m_startTime.secsTo(QDateTime::currentDateTime());
    else
        return 0;
}

QString CallProxy::reason() const
{
    TRACE;
    return m_reason;
}

bool CallProxy::multiparty() const
{
    TRACE;
    return m_multiparty;
}

void CallProxy::answer()
{
    TRACE;
    proceedCallAnswer();
}

void CallProxy::proceedCallAnswer()
{
    TRACE;

    QDBusPendingReply<QDBusObjectPath> reply;
    QDBusPendingCallWatcher *watcher;

    reply = Answer();
    watcher = new QDBusPendingCallWatcher(reply);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            SLOT(answerFinished(QDBusPendingCallWatcher*)));
}

void CallProxy::deniedCallAnswer()
{
    TRACE;

    // Hang up the incoming call, if resources to accept it are inavailabe
    hangup();

    emit ManagerProxy::instance()->callManager()->deniedCallAnswer();
}

void CallProxy::deflect(const QString toNumber)
{
    TRACE;

    QDBusPendingReply<QDBusObjectPath> reply;
    QDBusPendingCallWatcher *watcher;

    reply = Deflect(toNumber);
    watcher = new QDBusPendingCallWatcher(reply);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            SLOT(deflectFinished(QDBusPendingCallWatcher*)));
}

void CallProxy::hangup()
{
    TRACE;

    QDBusPendingReply<> reply;
    QDBusPendingCallWatcher *watcher;

    reply = Hangup();
    watcher = new QDBusPendingCallWatcher(reply);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            SLOT(hangupFinished(QDBusPendingCallWatcher*)));
}

void CallProxy::getPropertiesFinished(QDBusPendingCallWatcher *watcher)
{
    TRACE;

    QDBusPendingReply<QVariantMap> reply = *watcher;

    if (reply.isError()) {
        qCritical() << QString("Failed to connect to %1 for call %2:\n\t%3")
            .arg(staticInterfaceName())
            .arg(path())
            .arg(lastError().message());
        return;
    }

    QVariantMap props = reply.value();

    QString l_start;

    m_lineid = qdbus_cast<QString>(props["LineIdentification"]);
    m_name   = qdbus_cast<QString>(props["Name"]);
    m_state  = qdbus_cast<QString>(props["State"]);
    l_start  = qdbus_cast<QString>(props["StartTime"]);
    m_multiparty  = qdbus_cast<bool>(props["Multiparty"]);

    setStartTimeFromString(l_start);

    // Indicate for this instance, that we've actually performed at least
    // one round trip call to this VoiceCall and we are in sync with it
    m_connected = true;
}

void CallProxy::answerFinished(QDBusPendingCallWatcher *watcher)
{
    TRACE;
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (reply.isError())
        qCritical() << QString("Answer() Failed: %1 - %2")
            .arg(reply.error().name())
            .arg(reply.error().message());
}

void CallProxy::deflectFinished(QDBusPendingCallWatcher *watcher)
{
    TRACE;
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (reply.isError())
        qCritical() << QString("Deflect() Failed: %1 - %2")
            .arg(reply.error().name())
            .arg(reply.error().message());
}

void CallProxy::hangupFinished(QDBusPendingCallWatcher *watcher)
{
    TRACE;
    QDBusPendingReply<> reply = *watcher;
    if (reply.isError())
        qCritical() << QString("Hangup() Failed: %1 - %2")
            .arg(reply.error().name())
            .arg(reply.error().message());
}

void CallProxy::propertyChanged(const QString &in0, const QDBusVariant &in1)
{
    TRACE;

    if (in0 == "LineIdentification") {
        m_lineid = qdbus_cast<QString>(in1.variant());
        emit dataChanged();
    } else if (in0 == "State") {
        m_state  = qdbus_cast<QString>(in1.variant());
        emit stateChanged();
    } else if (in0 == "StartTime") {
        setStartTimeFromString(qdbus_cast<QString>(in1.variant()));
    } else if (in0 == "Multiparty") {
        m_multiparty = qdbus_cast<bool>(in1.variant());
        emit multipartyChanged();
    } else {
        qDebug() << QString("Unexpected property \"%1\" changed...").arg(in0);
    }
}

void CallProxy::disconnectReason(const QString &in0)
{
    TRACE;
    m_reason = in0;
    emit callDisconnected(in0);
}

void CallProxy::setStartTimeFromString(const QString &val)
{
    TRACE;
    if (val.isEmpty())
        return;

    m_startTime = qDateTimeFromOfono(val);

    if (!m_startTime.isValid())
        m_startTime = QDateTime::QDateTime::currentDateTime();
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
