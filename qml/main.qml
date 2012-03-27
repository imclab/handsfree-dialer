/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import com.tizen.hfdialer 1.0

import 'javascripts/framework.js' as Support

Item
{
    id: main

    width: 1024; height: 600

    Dialer { id: adapter }

    Component.onCompleted: {
      console.log("######## Completed loading component, initializing...");

      adapter.incomingCall.connect(function()
      {
        var call = adapter.currentCall;

        console.log("*** QML *** :: INCOMING CALL:" + call);
        console.log("*** QML *** ::   MSISDN: " + call.msisdn);
        console.log("*** QML *** ::    START: " + call.startedAt);
        console.log("");

        dialpage.activeCall = call       

      });

      if(adapter.currentCall)
      {
        dialpage.activeCall = call
      }

    }

    function showErrorMessage(mesg) {
        mesgDialog.mesg = mesg;
        mesgDialog.state = 'shown';
    }

    function dial(msisdn) {
        if(msisdn.trim().length == 0)
        {
            console.log("*** QML *** :: You can't dial without a number!");
            showErrorMessage(qsTr("No number specified!"));
            return false;
        }

        if (!adapter.modemOnline)
        {
          console.log("*** QML *** :: modem is not available or powered down");
          showErrorMessage(qsTr("modem is not available or powered down!"));
          return false;
        }

        console.log("*** QML *** :: Attempting to dial MSISDN: " + msisdn);

        dialpage.activeCall = {
          state: 'dialing',
          msisdn: msisdn
        };

        adapter.dial(msisdn);

        return true;
    }

    function dialMailbox() {
        if(adapter.mailbox) {
            console.log("*** QML *** :: Attempting to call mailbox number: " + adapter.mailbox);
            main.dial(adapter.mailbox);
        } else {
            console.log("*** QML *** :: No mailbox number defined!");
            showErrorMessage(qsTr("No mailbox number defined."));
        }
    }

    function dialSpeedDial(index) {
        if(adapter.speedDial(index))
        {
            console.log("*** QML *** :: Calling speed dial " + index + ": " + adapter.speedDial(index));
            main.dial(adapter.speedDial(index));
        } else {
            console.log("*** QML *** :: No speed dial number defined for: " + index);
            showErrorMessage(qsTr("No speed dial for " + (index + 1)));
        }
    }

    DialPage
    {
        id: dialpage
        anchors.fill: parent
    }

    MessageDialog {
        id: mesgDialog
        state: 'hidden'
    }
}

