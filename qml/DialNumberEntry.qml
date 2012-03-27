/*
 * dialer - QML User Interface Component
 *
 * Copyright (c) 2011, Tom Swindell.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

import Qt 4.7

Image
{
    property string placeHolderText: qsTr("Enter Number")
    property TextInput textInput: input

    id: root
    source: "/usr/share/hfdialer/images/ivi_textarea.png"
    function clear()
    {
        input.color = "#3B3A39";
        input.text = placeHolderText;
    }

    function isBlank()
    {
        return (input.text == placeHolderText);
    }

    function appendChar(character)
    {
        if(input.text == placeHolderText) {input.text = character} else {input.text += character};
    }

    TextInput
    {
        id: input
        anchors.centerIn: parent   
        color: "#3B3A39"
        cursorVisible: false
        activeFocusOnPress: false
        inputMethodHints: Qt.ImhDialableCharactersOnly
        font {pixelSize: 42}
        text: placeHolderText

        Component.onCompleted: forceActiveFocus();

        onTextChanged: {
            if(text.length == 0) root.clear();

            if(text.length > placeHolderText.length && text.substr(0, placeHolderText.length) == placeHolderText)
            {
              text = text.substr(placeHolderText.length);
            }

            if(text.length < placeHolderText.length && placeHolderText.substr(0, text.length) == text)
            {
              text = placeHolderText;
            }

            if(text == placeHolderText)
            {
                color = "#3B3A39";
            }
            else
            {
                color = "#3B3A39";
            }
        }

        onAccepted: {
          main.dial(text)
        }
    }
}
