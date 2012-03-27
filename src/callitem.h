/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef CALLITEM_H
#define CALLITEM_H

#include <QString>
#include <QDateTime>
#include <QtDBus>
//#include <QMediaPlayer>
#include <MGConfItem>
#include "callproxy.h"

    enum CallState {
        STATE_NONE = 0,
        STATE_ACTIVE,
        STATE_HELD,
        STATE_DIALING,
        STATE_ALERTING,
        STATE_INCOMING,
        STATE_WAITING,
        STATE_DISCONNECTED,
        STATE_LAST,
    };

    enum CallDirection {
        DIRECTION_NONE = 0,
        DIRECTION_IN,
        DIRECTION_OUT,
        DIRECTION_MISSED,
        DIRECTION_LAST,
    };

    enum CallDisconnectReason {
        DISCONNECT_NONE = 0,
        DISCONNECT_LOCAL,
        DISCONNECT_REMOTE,
        DISCONNECT_NETWORK,
        DISCONNECT_LAST,
    };

class CallItem: public QObject 
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path WRITE setPath)
    Q_PROPERTY(QString lineID READ lineID)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(CallState state READ state)
    Q_PROPERTY(CallDirection direction READ direction WRITE setDirection)
    Q_PROPERTY(CallDisconnectReason reason READ reason)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(QDateTime startTime READ startTime)
    Q_PROPERTY(bool multiparty READ multiparty)

    //Q_PROPERTY(PeopleItem* peopleItem READ peopleItem WRITE setPeopleItem)
    Q_PROPERTY(CallProxy* callProxy READ callProxy)

public:
    CallItem(const QString path = QString(), QObject *parent = 0);
    virtual ~CallItem();

    QString path() const;
    QString lineID() const;
    QString name() const;
    CallState state() const;
    CallDirection direction() const;
    CallDisconnectReason reason() const;
    int duration() const;
    QDateTime startTime() const;
    //PeopleItem *peopleItem() const;
    CallProxy *callProxy() const;
    bool isValid();
    bool isValid() const;
    bool multiparty();

public Q_SLOTS:
    void init();
    //void setPeopleItem(PeopleItem *person);
    bool setPath(QString path);  // Setting this will create the CallProxy
    void setDirection(CallDirection direction);
    void click();

    void silenceRingtone();

Q_SIGNALS:
    // TODO: handle tap-and-hold
    void clicked();
    void stateChanged();
    void dataChanged();
    void multipartyChanged();

private Q_SLOTS:
    void callStateChanged();
    void callDataChanged();
    void callDisconnected(const QString &reason);
//    void ringtoneStatusChanged(QMediaPlayer::MediaStatus status);
    void callMultipartyChanged();

private:
    //QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &val);
   // void populatePeopleItem();

    QString               m_path;
    //PeopleItem           *m_peopleItem;
    //QMediaPlayer         *m_ringtone;
    MGConfItem           *m_rtKey;
    bool                  m_isconnected;
    QString               m_ringtonefile;
    CallProxy		 *m_call;

    Q_DISABLE_COPY(CallItem)
};

#endif // CALLITEM_H
