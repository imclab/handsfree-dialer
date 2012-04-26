/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

import 'javascripts/framework.js' as Support

Item
{
    id: root

    anchors.fill: parent

    property variant call  
    property string  callDuration: "00:00:00"
    property string  callerLabelText: "Unknown Caller"
    state : 'disconnected'

    onCallChanged: {
     console.log("*** call changed in large, before if")   
     if(call && call.msisdn) {
            console.log("*** in calllarge if >> " + call.state );
	    
 	    root.state = call.state

          /*  if (call.name) {
                root.callerLabelText = call.name;


            } else {
            */
		if (call.numberLen <= 10)
		{
                  root.callerLabelText = call.msisdn[0] + call.msisdn[1] + call.msisdn[2] + '-' +
		  		         call.msisdn[3] + call.msisdn[4] + call.msisdn[5] + '-' +
                                         call.msisdn[6] + call.msisdn[7] + call.msisdn[8] + call.msisdn[9];
		}
		else
		  root.callerLabelText = call.msisdn;
           //}
        }
    }

    Timer {
        interval: 1000; repeat: true; running: true;

        onTriggered: {
            if(call) {
                if(call.duration && call.duration > 0)
                {
                    callDuration = Support.friendlyInterval(call.duration);
                }
            }
        }
    }

    states {
        State {
            name: 'active'           
	    PropertyChanges {target: root; callDuration: "00:00:00"}
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width}
            PropertyChanges {target: stateInd; text: qsTr("Active")}
        }

        State {
            name: 'held'          
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width}
            PropertyChanges {target: stateInd; text: qsTr("Held")}
        }

        State {
            name: 'dialing'           
 	    PropertyChanges {target: root; callDuration: "00:00:00"}
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width}
            PropertyChanges {target: stateInd; text: qsTr("Dialing...")}
        }

        State {
            name: 'alerting'           
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width}
            PropertyChanges {target: stateInd; text: qsTr("Alerting...")}
        }

        State {
            name: 'incoming'            
	    PropertyChanges {target: root; callDuration: "00:00:00"}
            PropertyChanges {target: answerButton; visible: true}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width * 0.45}
            PropertyChanges {target: stateInd; text: qsTr("Incoming...")}
        }

        State {
            name: 'waiting'            
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: true; width: parent.width}
            PropertyChanges {target: stateInd; text: qsTr("Waiting...")}
        }

        State {
            name: 'disconnected'    
	    PropertyChanges {target: callDurationInd; text: "00:00:00"}        
            PropertyChanges {target: answerButton; visible: false}
            PropertyChanges {target: hangupButton; visible: false}
            PropertyChanges {target: stateInd; text: qsTr("Disconnected")}
        }
    }

    Text
    {
        id: stateInd
        anchors {top: parent.top; topMargin: 20; horizontalCenter: parent.horizontalCenter}
        color: '#ffffff'
        font {pixelSize: 38}
	text: qsTr("Disconnected")
    }

    Text
    {
        id: callerInd
        anchors {top: stateInd.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter}
        color: '#ffffff'
        font {pixelSize: 75}
        text: callerLabelText
    }
    
    Image
    {
        id: clock
	source: "/usr/share/hfdialer/images/ivi_icon-time.png"
	anchors { right: callDurationInd.left; rightMargin: 2; bottom: callerInd.top}
	width: 25
	height: 25
	smooth: true
    }
    
    Text
    {
        id: callDurationInd
        anchors {bottom: callerInd.top; right: parent.right; topMargin: 10; rightMargin: parent.width * 0.2}
        font {pixelSize: 22}
        color: '#dfdfdf'
        text: callDuration
    }

    Item
    {
        id: buttons
        anchors {top: callerInd.bottom; topMargin: 15; left: parent.left; leftMargin: parent.width * 0.2; right: parent.right; rightMargin: parent.width * 0.2}
	
        width: parent.width * 0.75
        height: 72

        Image
        {
            id: answerButton
            height: 72
            width: parent.width *0.45
            anchors {left: parent.left;}
            source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept.png"
	    
            Image
            {
                width: 40; height: width
                anchors.centerIn: parent
                smooth: true
                source: "/usr/share/hfdialer/images/ivi_icon-call.png"
            }

            MouseArea
            {
                anchors.fill: parent

                onPressed: {
                    answerButton.source = "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept-active.png"
                }

                onReleased: {
                    answerButton.source = "/usr/share/hfdialer/images/ivi_btn-incomingcall-accept.png"
                }

                onClicked: {
                    console.log("*** QML *** :: Answering call");
                    adapter.currentCall.answer();
                }
            }
        }

        Image
        {
            id: hangupButton
            height: 72
	    width: parent.width * 0.45
            anchors {right: parent.right;}
            source: "/usr/share/hfdialer/images/ivi_btn-incomingcall-decline.png"

            Image
            {
                width: 40; height: width
                anchors.centerIn: parent
                smooth: true
                source: "/usr/share/hfdialer/images/ivi_icon-endcall.png"
            }

            MouseArea
            {
                anchors.fill: parent

                onPressed: {
                    hangupButton.source = "/usr/share/hfdialer/images/ivi_btn-incomingcall-decline-active.png"
                }

                onReleased: {
                    hangupButton.source = "/usr/share/hfdialer/images/ivi_btn-incomingcall-decline.png"
                }

                onClicked: {
                    console.log("*** QML *** :: Hanging up call");
		    root.parent.state = 'disconnected'
		    adapter.hangupAll();
                    root.state = 'disconnected'
		    root.parent.parent.state = 'disconnected'
                    if (root.parent.parent.call)
		    	root.parent.parent.call = null
		    if (root.call)
			root.call = null
                }
            }
        }
    }
}

