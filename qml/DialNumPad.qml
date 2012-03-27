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

    property bool numPadShown: true
    property DialNumberEntry entry
    	
    height: parent.height
    property real buttonHeight: (parent.height / 5) - 41;

    function insertText(text)
    {
        entry.appendChar(text)
    }

    function show()
    {
        height = 72 * 5 + 4;
        numPadShown = true;
    }

    function hide()
    {
        height = 72;
        numPadShown = false;
    }

    function toggle()
    {
        if(numPadShown == true) {hide()} else {show()}
    }

    Behavior on height {PropertyAnimation {duration: 500; easing.type: Easing.OutBounce}}

    Image
    {
        id: numpad
        width: parent.width; height: childrenRect.height + 21;
	source: "/usr/share/hfdialer/images/ivi_buttonarea.png"
        Behavior on opacity {PropertyAnimation {duration: 500}}

        Column
        {
           id: columnBox
           anchors {top: parent.top; right: parent.right; left: parent.left; margins: 11}
	   spacing: 5

            Row
            {
                width: parent.width
                anchors {bottomMargin: 5}
		spacing: 5

                DialNumPadButton {
		    id: dial1
                    text: qsTr("1");
                    height: buttonHeight;
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialMailbox();
                }
                DialNumPadButton {
                    text: qsTr("2");
                    height: buttonHeight;		
              	    detail: qsTr("abc");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(1);
                }
                DialNumPadButton {
                    text: qsTr("3");
                    height: buttonHeight;		
               detail: qsTr("def");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(2);
                }
            }
            Row
            {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                DialNumPadButton {
                    text: qsTr("4");
                    height: buttonHeight;
		    detail: qsTr("ghi");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(3);
                }
                DialNumPadButton {
                    text: qsTr("5");
                   height: buttonHeight;
		   detail: qsTr("jkl");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(4);
                }
                DialNumPadButton {
                    text: qsTr("6");
                   height: buttonHeight;
		   detail: qsTr("mno");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(5);
                }
            }
            Row
            {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
              spacing: 5
                  DialNumPadButton {
                    text: qsTr("7");
                   height: buttonHeight;
		   detail: qsTr("pqrs");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(6);
                }
                DialNumPadButton {
                    text: qsTr("8");
                   height: buttonHeight;
		   detail: qsTr("tuv");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(7);
                }
                DialNumPadButton {
                    text: qsTr("9");
                  height: buttonHeight;
		    detail: qsTr("wxyz");
                    onClicked: root.insertText(text);
                    onPressAndHold: main.dialSpeedDial(8);
                }
            }
            Row
            {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
              spacing: 5
                  DialNumPadButton {
                    text: qsTr("*");
                   height: buttonHeight;
		   onClicked: root.insertText(text);
                    onPressAndHold: root.insertText("p");
                }
                DialNumPadButton {
                    text: qsTr("0");
                  height: buttonHeight;
		    detail: qsTr("+");
                    onClicked: root.insertText(text);
                    onPressAndHold: root.insertText("+");
                }
                DialNumPadButton {
                    text: qsTr("#");
                 height: buttonHeight;
		     onClicked: root.insertText(text);
                    onPressAndHold: root.insertText("w");
                }
            }
        
   

    Row
    {
        id: actions
        width: parent.width; height: dial1.height

	spacing: 5
            Image {
                id: bDelete;               
                width: parent.width / 4; height: parent.height
                source: "/usr/share/hfdialer/images/ivi_btn-delete.png"
                Image {	
		   anchors{ left: bDelete.left}
		   height: parent.height
		   width: parent.width
		   source: "/usr/share/hfdialer/images/ivi_icon-delete.png"
		   fillMode: Image.PreserveAspectFit
		} 
            	MouseArea
		{
		   anchors.fill: parent
		   onClicked: 
		   {
			if(entry.textInput.text == entry.placeHolderText)
			  return;
			
			entry.textInput.text = entry.textInput.text.substring(0, entry.textInput.text.length -1);
		   }
		 
		   onPressAndHold: entry.clear();

		}
	    }
            
	    Image {
                id: bCall;

		height: parent.height
                width: parent.width - bDelete.width - 5	
		source: "/usr/share/hfdialer/images/ivi_btn-call.png"

                Image {
		  anchors { centerIn: parent}
		  height: parent.height
		  width: parent.width
		  source: "/usr/share/hfdialer/images/ivi_icon-call.png"
		  fillMode: Image.PreserveAspectFit
		}

		MouseArea{
		anchors.fill: parent
		onClicked: {
                    if(entry.isBlank())
                    {
                        console.log("*** QML *** :: You can not dial without a number!");
                        main.showErrorMessage(qsTr("You can't dial without a number!"));
                        return;
                    }

                    if(main.dial(entry.textInput.text))
                    {
                        entry.clear();
                    }
                }
		}
            }
         }
	}
   }
}
