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

    signal replyRequestPidCode(string reply)
    signal cancelRequest()
    property string deviceName
    property string replyValue: legacyPairing ? "0000" : Math.floor(Math.random()*999999)
    property bool legacyPairing: false

    Component.onCompleted: {
        numPad.pidRequest = true
        numPad.pidEdit = textInputField
    }

    Column {
        width: parent.width - 15
        anchors {centerIn: parent}
        spacing: 10

        Text {
            id: textlabel
            text: qsTr("Enter the following code on %1").arg(deviceName)
            width: parent.width
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
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

        Row {
            id: buttonGroup
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            width: parent.width
            height: 50

            Image {
                id: acceptButton
                source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept.png"
                width: buttonGroup.width / 2 - 5
                height: parent.height

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(deviceName + " replying with key: " + textInputField.text)
                        numPad.pidRequest = false
                        replyRequestPidCode(textInputField.text);
                    }
                }

                Text {
                    id: acceptButtonText
                    text: qsTr("Accept")
                    anchors.centerIn:parent
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    color: "white"
                }
            }

            Image {
                id: rejectButton
                source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-decline.png"
                width: buttonGroup.width / 2 - 5
                height: parent.height

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(deviceName + " replying with key: " + textInputField.text)
                        numPad.pidRequest = false
                        cancelRequest()
                    }
                }

                Text {
                    id: cancelButtonText
                    text: qsTr("Cancel")
                    anchors.centerIn:parent
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    color: "white"
                }
            }


        }

    }
    ///we do this because this property is actually set post onCompleted:
    onLegacyPairingChanged: {
        console.log("legacy pair? " + legacyPairing)
        if(!legacyPairing) {
            replyRequestPidCode(textInputField.text);
            console.log(deviceName + " replying with key: " + replyValue)
        }
    }

}
