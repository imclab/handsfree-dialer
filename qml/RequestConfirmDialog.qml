/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Rectangle {
    id: root
    anchors.fill: parent
    color: "#51504F"

    property string deviceName: ""
    property string key: ""
    signal replyRequestConfirmation(bool reply)

Column {
    id: container
    width: parent.width - 15
    anchors {centerIn: parent}
    spacing: 10
    Component.onCompleted: {
        console.log("request confirm dialog height: " + container.height + " width = " + container.width + " name = " + root.deviceName + " code = " + root.key)
    }

    Text {
        id: textlabel
        width: parent.width
        height: paintedHeight
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Pair with %1 with key %2?").arg(root.deviceName).arg(root.key)
        wrapMode: Text.WordWrap
        font.pixelSize: 24
        color: "White"
    }

    Row {
        id: buttonGroup
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        height: 50

        Image {
            id: acceptButton
            source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept.png"
            height: parent.height
            width:  root.width / 2 - 20

            MouseArea {
                  anchors.fill: parent
            onClicked: {
                replyRequestConfirmation(true);
            }
            }
            Text {
                 text: qsTr("Accept")
                 anchors.centerIn:parent
                 font.pointSize: 14
                 color: "white"
             }
        }

        Image {
            id: rejectButton
            source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-decline.png"

            height: parent.height
            width:  root.width / 2 - 20

            MouseArea {
                  anchors.fill: parent
            onClicked: {
                replyRequestConfirmation(false);
            }
            }

            Text {                 
                 text: qsTr("Reject")
                 anchors.centerIn:parent
                 font.pointSize: 14
                 color: "white"
             }
        }
    }

}
}
