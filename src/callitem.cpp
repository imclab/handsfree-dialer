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
#include "callitem.h"
#include "dialerapplication.h"
#include "pacontrol.h"
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QDebug>

#define DEFAULT_RINGTONE "ring-1.wav"

CallItem::CallItem(const QString path, QObject *parent)
    : QObject(parent),
      m_path(path),
      m_rtKey(new MGConfItem("/apps/dialer/defaultRingtone")),
      m_isconnected(FALSE)
{
    TRACE;
    QString l_ringtoneFile = QString("/usr/share/hfdialer/sounds/%1").arg(DEFAULT_RINGTONE);
    QString l_rtKeyValue = m_rtKey->value(QVariant(l_ringtoneFile)).toString();

    if (!QFileInfo(l_rtKeyValue).exists()) {
        qWarning() << QString("CallItem: %1 ringtone not found: %2")
            .arg(m_path)
            .arg(l_rtKeyValue);
    }

    if (isValid()) {
        init();
    }
}

CallItem::~CallItem()
{
    TRACE;
    PAControl::instance()->unrouteAudio();
   
    if (m_rtKey) {
        delete m_rtKey;
    }
    m_rtKey = 0;

    // delete the callproxy object
    if (callProxy())
        delete callProxy();
}

void CallItem::init()
{
    TRACE;
    if (!m_path.isEmpty()) {
        m_call = new CallProxy(m_path);
        if (m_call->isValid()) {
            // dynamic_cast<CallItemModel*>(model())->setCall(call);
            connect(m_call,SIGNAL(stateChanged()),this,SLOT(callStateChanged()));
            connect(m_call,SIGNAL(dataChanged()),this,SLOT(callDataChanged()));
            connect(m_call,SIGNAL(multipartyChanged()),this,SLOT(callMultipartyChanged()));
        } else {
            qCritical("Invalid CallProxy instance!");
        }
    } else {
        qCritical("Empty call path.  Can not create CallProxy!");
    }
}

bool CallItem::isValid()
{
    TRACE;
    return (!path().isEmpty());
}

bool CallItem::isValid() const
{
    TRACE;
    return (!path().isEmpty());
}

QString CallItem::path() const
{
    TRACE;
    return m_path;
}

bool CallItem::setPath(QString path)
{
    TRACE;
    if (!m_path.isEmpty()) {
        qCritical("Path already set and can not be changed once it is set");
        return false;
    } else if (path.isEmpty()) {
        qCritical("It makes no sense to set Path to an empty string!?!?");
        return false;
    }

    m_path = path;

    init();

    return true;
}

void CallItem::setDirection(CallDirection direction)
{
    TRACE; 
}

QString CallItem::lineID() const
{
    TRACE;
    return m_call->lineID();
}

QString CallItem::name() const
{
    TRACE;
    return m_call->name();
}

CallState CallItem::state() const
{
    TRACE;
    CallState cs = STATE_NONE;
    QString state = m_call->state();

    if (state == "active")
        cs = STATE_ACTIVE;
	   
    else if (state == "held")
        cs = STATE_HELD;
    else if (state == "dialing")
        cs = STATE_DIALING;
    else if (state == "alerting")
        cs = STATE_ALERTING;
    else if (state == "incoming")
        cs = STATE_INCOMING;
    else if (state == "waiting")
        cs = STATE_WAITING;
    else if (state == "disconnected")
        cs = STATE_DISCONNECTED;

    return cs;
}

CallDirection CallItem::direction() const
{
    TRACE;
    return DIRECTION_NONE;
}

CallDisconnectReason CallItem::reason() const
{
    TRACE;
    return DISCONNECT_NONE; 
}

int CallItem::duration() const
{
    TRACE;
    return m_call->duration();
}

QDateTime CallItem::startTime() const
{
    TRACE;
    return m_call->startTime();
}

CallProxy* CallItem::callProxy() const
{
    TRACE;
    return m_call;
}

void CallItem::click()
{
    TRACE;

    emit clicked();
}

void CallItem::silenceRingtone()
{
    TRACE;
}

void CallItem::callStateChanged()
{
    TRACE;
    emit stateChanged();
}

void CallItem::callDataChanged()
{
    TRACE;
}

void CallItem::callDisconnected(const QString &reason)
{
    TRACE;
    Q_UNUSED(reason);
}


bool CallItem::multiparty()
{
    TRACE; 
    return false;   
}

void CallItem::callMultipartyChanged()
{
    TRACE;
    emit multipartyChanged();
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
