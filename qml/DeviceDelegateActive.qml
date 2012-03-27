/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
Item {
    id: root
    property string deviceName
    property string address
    property string icon: ""
    property string alias: ""
    property string dbuspath: ""
    property variant uuids: []

    property int containerHeight: 80
    height: containerHeight
    width: parent.width
    signal clicked()
    signal close()
    Image {
        id: availableBluetoothItem

        source: "/usr/share/hfdialer/images/ivi_btn-list-inactive.png"
        
        anchors {fill: parent; leftMargin: 8; rightMargin: 8; topMargin: 8}

        Text {
            id: mainText

            anchors {left: parent.left; top: parent.top; bottom: parent.bottom; leftMargin: 15}
            width: parent.width * 0.75

            verticalAlignment: Text.AlignVCenter
            height:  availableBluetoothItem.containerHeight
            font.pixelSize: parent.height / 2
            style: Text.Outline
            styleColor: "#3B3A39"
            color: "white"            
            text: root.deviceName
            elide: Text.ElideRight
        }

        Image {
            id: closeButton
            source: "/usr/share/hfdialer/images/ivi_icon-list-delete.png"
            anchors { left: mainText.right; right: parent.right; top: parent.top; bottom: parent.bottom}

            MouseArea {
                id: closeArea
                  anchors.fill: parent

            onPressed: {
                closeButton.source = "/usr/share/hfdialer/images/ivi_icon-list-delete-active.png"
            }

            onReleased: {
                closeButton.source = "/usr/share/hfdialer/images/ivi_icon-list-delete.png"
            }

            onClicked: {
                console.log("CLOSE BUTTON CLICKED")
                root.close()
                }
            }
        }
    }
}
