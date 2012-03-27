/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item
{
    id: root

    property Item numpad

    property string text: ''
    property string detail: ''

    property int marginSize: 10

    signal clicked
    signal pressAndHold

    width: (parent.width - marginSize) / 3; height: 72

    SystemPalette {id: palette; colorGroup: SystemPalette.Active}

    Image
    {
        id: buttonImg
        anchors {fill: parent;}                       
        source: "/usr/share/hfdialer/images/ivi_btn-numbers.png"

    }

    Text
    {
        width: parent.width
        height: parent.height * 0.6
        anchors {centerIn: parent}
        text: parent.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font {pixelSize: parent.height / 2}
        color: "white"
	style: Text.Outline;
	styleColor: "#3B3A39"
    }

    Text
    {    
        anchors {top: parent.top; right: parent.right; topMargin: 21; rightMargin: 17}
        text: parent.detail       
        font {pixelSize: 15}
        color: "#D8D8D8"
    }

    MouseArea
    {
        anchors.fill: parent

        onPressed: {
            buttonImg.source = "/usr/share/hfdialer/images/ivi_btn-numbers-active.png"
        }
	
	onReleased: {
	    buttonImg.source = "/usr/share/hfdialer/images/ivi_btn-numbers.png"        
	}
 
        onClicked: {
            root.clicked();
        }
        onPressAndHold: root.pressAndHold();
    }
}
