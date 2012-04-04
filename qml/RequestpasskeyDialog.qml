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


    signal replyRequestPasskey(int reply)
    property string deviceName

    Component.onCompleted: {
        numPad.pidRequest = true
        numPad.pidEdit = textInputField
    }

Column {
    width: parent.width - 15
    spacing: 10

    Text {
        id: textlabel
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Enter passcode to use:")
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        font.pixelSize: 24
        color: "white"
    }

    Rectangle {
        id: textInput
        anchors.horizontalCenter: parent.horizontalCenter
        height: 40
        width: parent.width
        color: "white"
        radius: 5

        TextInput {
            id: textInputField           
            anchors.centerIn: parent
            width: parent.width
            height: parent.height * 0.75
            font.pixelSize: 24
            color: "black"
            text: replyValue
            horizontalAlignment: Text.AlignHCenter
            activeFocusOnPress: false
        }
    }

   Image {
        id: acceptButton
        source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept.png"
        anchors.horizontalCenter: parent.horizontalCenter
        width: textInput.width
        height: 50

        MouseArea {
              id: mouseArea
              anchors.fill: parent

        onClicked: {
            numPad.pidRequest = false
            nearbyDevicesModel.replyRequestPasskey(textInputField.text);
            }
        }
    }

   Text {
        id: text
        text: qsTr("Accept")
        anchors.centerIn:parent
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        color: "white"
    }
}
}
