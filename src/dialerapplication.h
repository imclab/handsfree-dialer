/*
 * hfdialer - Hands Free Voice Call Manager
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef DIALERAPPLICATION_h
#define DIALERAPPLICATION_h

#include "managerproxy.h"
#include <QSortFilterProxyModel>
#include <QApplication>

class DialerApplication: public QApplication
{
    Q_OBJECT;

    Q_PROPERTY(bool isConnected READ isConnected);
    Q_PROPERTY(bool hasError READ hasError);
    Q_PROPERTY(QString lastError READ lastError);

public:
    DialerApplication(int &argc, char **argv);

    static DialerApplication* instance();

    virtual void releasePrestart();
    virtual void restorePrestart();

    bool hasError() const;
    QString lastError() const;
    void setError(const QString msg);

    bool isConnected();
    
Q_SIGNALS:
    void showUi();
    void hideUi();

private Q_SLOTS:
    void connectAll();
    void messagesWaitingChanged();

    void modemChanged();
    void modemConnected();
    void modemDisconnected();
    void modemInterfacesChanged(QStringList interfaces);
    void modemPowered(bool isPowered);
    void networkConnected();
    void networkDisconnected();
    void callManagerConnected();
    void callManagerDisconnected();

    void onCallsChanged();

private:
    void init();

    ManagerProxy *m_manager;
    ModemProxy   *m_modem;
    NetworkProxy *m_network;
    CallManager  *m_callManager;

    bool          m_connected;
    QString       m_lastError;

    Q_DISABLE_COPY(DialerApplication);
};

#endif // DIALERAPPLICATION_H

/* Local Variables:      */
/* mode:c++              */
/* c-basic-offset:4      */
/* indent-tabs-mode: nil */
/* End:                  */
