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

            anchors {fill: parent; leftMargin: 15}
            verticalAlignment: Text.AlignVCenter
            height:  availableBluetoothItem.containerHeight
            font.pixelSize: parent.height / 2
            style: Text.Outline
            styleColor: "#3B3A39"
            color: "white"            
            text: root.deviceName
            elide: Text.ElideRight
        }

    }

    MouseArea {
        id: mArea
          anchors.fill: parent

          onPressed: {
              availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list.png"
          }
          onReleased: {
              availableBluetoothItem.source = "/usr/share/hfdialer/images/ivi_btn-list-inactive.png"
              root.clicked()
          }
    }
}
