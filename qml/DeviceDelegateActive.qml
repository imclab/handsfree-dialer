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

    Connections {
        target: adapter
        onModemOnlineChanged: {

            //If the modem gets powered down for any reason, attempt to power it again to maintain connection
            if (!adapter.modemOnline)
            {
                mainText.color = "grey"
                availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list-inactive.png"
            }
            else
            {
                mainText.color = "white"
                availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list.png"
            }
         }
    }

    Image {
        id: availableBluetoothItem

        source: !adapter.modemOnline? "/usr/share/hfdialer/images/ivi_btn-list-inactive.png" : availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list.png"
        anchors {fill: parent; leftMargin: 8; rightMargin: 8; topMargin: 8}

	MouseArea {
          id: clickArea
          anchors.fill: parent

           onPressed: {
        	 availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list.png"            
            }

            onReleased: {
	         availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list-inactive.png"      
            }

            onClicked: {
                adapter.modemOnline = true
            }
        }

        Text {
            id: mainText

            anchors {left: parent.left; top: parent.top; bottom: parent.bottom; leftMargin: 15}
            width: parent.width * 0.75

            verticalAlignment: Text.AlignVCenter
            height:  availableBluetoothItem.containerHeight
            font.pixelSize: parent.height / 2
            style: Text.Outline
            styleColor: "#3B3A39"
            color: !adapter.modemOnline? "grey" : "white"
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
