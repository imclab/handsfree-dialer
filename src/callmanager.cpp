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
#include "callmanager.h"

class CallManagerPrivate
{
public:
    //ResourceProxy *resource;
    QHash<QString, CallItem *>  callItems;
};

CallManager::CallManager(const QString &modemPath, QObject *parent)
    : OfonoVoiceCallManager(OfonoModem::AutomaticSelect, modemPath, parent),
      d(new CallManagerPrivate)
{
    TRACE;

    // Transform existing calls list, into list of CallItems
    updateCallItems();

    // Begin tracking calls
    connect(this, SIGNAL(callAdded(const QString)),
            this, SLOT(addCall(const QString)));
    connect(this, SIGNAL(callRemoved(const QString)),
            this, SLOT(removeCall(const QString)));

    // Hook into parent class signals
    connect(this, SIGNAL(dialComplete(const bool)),
            this, SLOT(dialFinished(const bool)));
    connect(this, SIGNAL(swapCallsComplete(const bool)),
            this, SLOT(swapFinished(const bool)));
    connect(this, SIGNAL(hangupAllComplete(const bool)),
            this, SLOT(hangupAllFinished(const bool)));
    connect(this, SIGNAL(sendTonesComplete(const bool)),
            this, SLOT(sendTonesFinished(const bool)));
    connect(this, SIGNAL(holdAndAnswerComplete(const bool)),
            this, SLOT(holdAndAnswerFinished(const bool)));
    connect(this, SIGNAL(transferComplete(const bool)),
            this, SLOT(transferFinished(const bool)));
    connect(this, SIGNAL(releaseAndAnswerComplete(const bool)),
            this, SLOT(releaseAndAnswerFinished(const bool)));
    connect(this, SIGNAL(privateChatComplete(const bool)),
            this, SLOT(privateChatFinished(const bool)));
    connect(this, SIGNAL(createMultipartyComplete(const bool)),
            this, SLOT(createMultipartyFinished(const bool)));
    connect(this, SIGNAL(hangupMultipartyComplete(const bool)),
            this, SLOT(hangupMultipartyFinished(const bool)));

    connect(this,SIGNAL(callsChanged()),this,SLOT(callChangedSlot()));

    connect(this, SIGNAL(validityChanged(bool)), this, SLOT(modemValidityChanged(bool)) );

    if (isValid())
        emit connected();
}

CallManager::~CallManager()
{
    TRACE;
    // FIXME: Do something here!!!
    qDebug() << QString("Destroying VoiceCallManager");
    qDebug() << QString("Purging all CallItems");
    foreach (CallItem *item, d->callItems) {
        disconnect(item, SIGNAL(stateChanged()));
        disconnect(item, SIGNAL(dataChanged()));
        delete item;
    }
    if (d->callItems.size() > 0)
        emit callsChanged();
    d->callItems.clear();
}

void CallManager::modemValidityChanged(bool valid)
{
    TRACE;
    if (valid)
	emit connected();
}

QList<CallItem *> CallManager::getCallItems() const
{
    TRACE;
    return d->callItems.values();
}

int CallManager::callCount() const
{
    TRACE;
    qDebug()<<"call count is currently = "<<d->callItems.size();
    return d->callItems.size();
}

int CallManager::multipartyCallCount() const
{
    TRACE;
    int call_count = 0;
    foreach (CallItem *c, d->callItems) {
        if(c->multiparty()) {
            call_count++;
        }
    }
    return call_count;
}

CallItem *CallManager::activeCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_ACTIVE)
                return c;
    return NULL;
}

CallItem *CallManager::heldCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_HELD)
                return c;
    return NULL;
}

CallItem *CallManager::dialingCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_DIALING)
                return c;
    return NULL;
}

CallItem *CallManager::incomingCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_INCOMING)
                return c;
    return NULL;
}

CallItem *CallManager::waitingCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_WAITING)
                return c;
    return NULL;
}

CallItem *CallManager::alertingCall() const
{
    TRACE;
    if (d->callItems.size())
        foreach (CallItem *c, d->callItems)
            if (c->state() == STATE_ALERTING)
                return c;
    return NULL;
}

void CallManager::setActiveCall(const CallItem &call)
{
    TRACE;
    swapCalls();
}

void CallManager::dial(const QString &number)
{
    TRACE;
    // Nothing to do if the modem is not powered up

    if(!modem()->powered()) {
        emit callsChanged();
        return;
    }

    // If not online (flight mode?), check if the requested number is
    // one of the allowed EmergencyNumbers, in which case, continue.
    // Otherwise, notify that only Emergency calls are permitted.
    if(!modem()->online()) {
        if(modem()->powered() && !emergencyNumbers().contains(number)) {
            emit callsChanged();
            emit onlyEmergencyCalls();
            return;
        }
    }

    proceedCallDial(number);
}

void CallManager::privateChat(const CallItem &call)
{
    TRACE;
}

/*
 * Resource Policy Manager Handler slots
 */
void CallManager::deniedCallDial()
{
    TRACE;
    qCritical() << QString("Denied: Dial resource");
}

void CallManager::lostCallDial()
{
    TRACE;
    qCritical() << QString("Lost: Dial resource");
    hangupAll();
}

void CallManager::proceedCallDial(const QString number)
{
    TRACE;
    OfonoVoiceCallManager::dial(stripLineID(number), QString());
}

void CallManager::deniedCallAnswer()
{
    TRACE;
    qCritical() << QString("Denied: Call resource");
    hangupAll();
}

void CallManager::deniedIncomingCall(CallItem *call)
{
    TRACE;

    qCritical() << QString("Denied: Incoming Call resource");
    qDebug() << QString("Insert new CallItem %1").arg(call->path());
    emit callsChanged();
    emit incomingCall(call);
}

void CallManager::lostIncomingCall(CallItem *call)
{
    TRACE;
    Q_UNUSED(call)
        qCritical() << QString("Lost: Incoming Call resource");
}

void CallManager::proceedIncomingCall(CallItem *call)
{
    TRACE;
    qDebug() << QString("Acquired: Incoming Call resource");
    qDebug() << QString("Insert new CallItem %1").arg(call->path());
    emit callsChanged();
    emit incomingCall(call);
}

/*
 * Private slots for async replies
 */

void CallManager::updateCallItems()
{
    TRACE;
    bool changed = false;

    // If ofono call list is empty (no calls), empty our CallItem list too.
    if (getCalls().isEmpty() && !d->callItems.isEmpty()) {
        qDebug() << QString("Purging all CallItems");
        foreach (CallItem *item, d->callItems)
            delete item;
        d->callItems.clear();
        emit callsChanged();
        return;
    }

    // Remove CallItems that are not in the ofono "calls" list
    QMutableHashIterator<QString, CallItem*> iter(d->callItems);
    while (iter.hasNext()) {
        CallItem *item = iter.next().value();
        // This item is not in the ofono list, remove it
        if (!getCalls().contains(item->path())) {
            qDebug() << QString("Removing old CallItem %1").arg(item->path());
            delete item;
            iter.remove();
            changed = true;
        }
    }

    // Insert new CallItems for paths in the ofono "calls" list we are missing
    foreach (QString path, getCalls()) {
        // Insert a new CallItem
        if (!d->callItems.contains(path)) {
            qDebug() << QString("Inserting new CallItem %1").arg(path);
            CallItem *call = new CallItem(path);
            connect (call, SIGNAL(stateChanged()),
                     this, SLOT(callStateChanged()));
            connect (call, SIGNAL(multipartyChanged()),
                     this, SLOT(callMultipartyChanged()));
            d->callItems.insert(path, call);

            // NOTE: Must explicity bubble this up since incoming and waiting
            //       calls do not "changeState" unless they are handled or
            //       timeout
            if ((call->state() == STATE_INCOMING) ||
                (call->state() == STATE_WAITING)) {
                proceedIncomingCall(call);
            } else {
                changed = true;
            }
        }
    }

    if (changed)
        emit callsChanged();
}

void CallManager::addCall(const QString &path)
{
    TRACE;
    qDebug() << QString("CallAdded: \"%1\"").arg(path);
    qDebug() <<"Call number is now "<< callCount();
    updateCallItems();
    emit callCountChanged(callCount());
}

void CallManager::removeCall(const QString &path)
{
    TRACE;
    qDebug() << QString("CallRemoved: \"%1\"").arg(path);
    qDebug() <<"Call number is now "<< callCount();
    updateCallItems();
    emit callCountChanged(callCount());
}

void CallManager::dialFinished(const bool status)
{
    TRACE;
    qDebug() <<"Call number is now "<< callCount();
 
    if (!status) {
        qCritical() << QString("dial() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
        // Fix BMC#10848:
        // Notify that state of the call has changed when the dialing fails
        emit callsChanged();
    }
}

void CallManager::hangupAllFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("hangupAll() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());

    //If there are still calls for some reason, delete them.
    if (callCount() > 0)
        {
            foreach (CallItem *item, d->callItems) {
                disconnect(item, SIGNAL(stateChanged()));
                disconnect(item, SIGNAL(dataChanged()));
                delete item;
            }
    
            d->callItems.clear();
            callCount();
            emit callsChanged();
        }
    callCount();
}

void CallManager::swapFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("swapCalls() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::holdAndAnswerFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("HoldAndAnswer() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::transferFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("Transfer() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::releaseAndAnswerFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("ReleaseAndAnswer() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::privateChatFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("PrivateChat() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::createMultipartyFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("CreateMultiparty() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::hangupMultipartyFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("HangupMultiparty() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::sendTonesFinished(const bool status)
{
    TRACE;
    if (!status)
        qCritical() << QString("SendTones() Failed: %1 - %2")
            .arg(errorName())
            .arg(errorMessage());
}

void CallManager::callStateChanged()
{
    CallItem *call = dynamic_cast<CallItem *>(sender());
    qDebug() << QString("%1 (%2) state has changed to %3")
        .arg(call->path())
        .arg(call->lineID())
        .arg(call->state());
    qDebug() << "number of calls "<< callCount();
    emit callsChanged();
}

void CallManager::callMultipartyChanged()
{
    TRACE;
    emit callsChanged();
    emit multipartyCallCountChanged(multipartyCallCount());
}

void CallManager::callChangedSlot()
{
    TRACE
    qDebug()<<"callChanged called!";
}

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
