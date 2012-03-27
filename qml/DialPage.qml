/*
 * Copyright 2012 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

import Tizen.Bluetooth 0.0 

Item
{
    id: root
    width: parent.width; height: parent.height

    property alias activeCall: activeCallView.call
    property alias callState: activeCallView.state

    Keys.onEscapePressed: {
        console.log("Escape Pressed");
        Qt.quit()
    }

    onCallStateChanged: {
        if (activeCallView.state != 'disconnected')
            dialPage.state = 'activeCall'
        else
            dialPage.state = 'noCall'
    }

    Image
    {
        id: dialPage
        anchors.fill: parent
        source: "/usr/share/hfdialer/images/ivi-v1_ivi-background.jpg"
        state: 'noCall'

        DialNumberEntry
        {
            id: numberEntry
            anchors {
                top: parent.top; //bottom: numPad.top
                left: parent.left; margins: 15 //right: parent.right
                //margins: 10
            }
            width: parent.width * 0.60
            height: parent.height / 4.5
        }

        DialNumPad
        {
            id: numPad
            width: numberEntry.width
            anchors {top: numberEntry.bottom; left: parent.left; bottom: parent.bottom; margins: 15}
            entry: numberEntry
        }
        states{
            State {
                name: 'activeCall'
                PropertyChanges {target: numPad; visible: false}
                PropertyChanges {target: numberEntry; visible: false}
                PropertyChanges {target: modemList; visible: false}
                PropertyChanges {target: vertDivider; visible: false}
            }
            State {
                name: 'noCall'
                PropertyChanges {target: numPad; visible: true}
                PropertyChanges {target: numberEntry; visible: true}
                PropertyChanges {target: modemList; visible: true}
                PropertyChanges {target: vertDivider; visible: true}
            }


        }

        CallItemView
        {
            id: activeCallView
        }

        Rectangle {

            id: vertDivider
            anchors {left: numberEntry.right; margins: 8}
            width: 4
            height: parent.height
            color: "#262626"

        }

        Rectangle {
            id: modemList
            anchors {left: vertDivider.right; right: parent.right; top: parent.top; bottom: parent.bottom}
            color: "#51504F"

            Text {
                id: yourDevicesTxt
                text: "Your Devices"
                font.pixelSize: 42
                color: "white"
                anchors {top: parent.top; right: parent.right; left: parent.left; bottom: horizDivider1.top; leftMargin: 15}

            }

            Rectangle {
                id: horizDivider1
                anchors {left: parent.left; right: parent.right; topMargin: 8; bottomMargin: 8;}
                y: 62
                height: 3
                color: "#B3BF3C"
            }

            Text {
                id: moreDevicesTxt
                text: "More Devices"
                font.pixelSize: 42
                color: "white"
                anchors {right: parent.right; left: parent.left; bottom: horizDivider2.top; leftMargin: 15}
                height: yourDevicesTxt.height
            }

            Rectangle {
                id: horizDivider2
                anchors {left: parent.left; right: parent.right; topMargin: 8; bottomMargin: 8;}
                y: parent.height / 2
                height: 3
                color: "#B3BF3C"
            }

            Component.onCompleted: {
                nearbyDevicesModel.discover(true);
                console.log("Devices qml has been created, checking for BT: " + btDevicesModel.adapterPresent)

            }

            Component.onDestruction: {
                nearbyDevicesModel.discover(false);
            }

            BluetoothDevicesModel {
                id: btDevicesModel
                property bool successFullPair: false
                onDevicePaired: {

                }
            }

            NearbyDevicesModel {
                id: nearbyDevicesModel

                property bool discovering: false

                Component.onCompleted: {

                }

                onRequestConfirmation: {
                    console.log("onRequestConfirm called")
                }

                onRequestPasskey: {
                    console.log("onRequestPasskey called")

                }

                onRequestPidCode: {
                    console.log("onRequestPidCode called")
                }

                onAdapterPropertiesChanged: {

                    if(name == "Discovering") {
                        discovering = value;
                    }

                }

                onNearbyDeviceFound: {
                    //console.log("new device: " + nearbyDevicesModel.alias(index))
                }

            }

            Flickable{
                id: activeFlickable

                anchors {top: horizDivider1.bottom; bottom: moreDevicesTxt.top; left: parent.left; right: parent.right}
                clip: true
                contentWidth: parent.width
                contentHeight: parent.height
                flickableDirection: Flickable.VerticalFlick

                Column {
                    id: deviceList
                    width: parent.width
                    spacing: 2
                    Repeater {
                        model: btDevicesModel
                        delegate: DeviceDelegateActive {

                            deviceName: model.name
                            address: model.address
                            dbuspath: model.path
                            uuids: model.profiles
                            property BluetoothDevice device: btDevicesModel.device(dbuspath)

                            Connections {
                                target: btDevicesModel
                                onDevicePaired: {
                                    console.log("new paired device address:" + device.address + "==" + model.address)
                                    if(device.address == model.address){
                                        device.trusted = true
                                    }
                                }

                                onConnectedChanged: {
                                    console.log("device CONNECTED, attempting to hook up audio")
                                    // device.connectAudio()
                                    // device.connectAudioSrc()
                                    // device.connectInput()
                                }
                            }

                            onClose: {
                                console.log("unparing ...");
                                device.unpair();
                            }
                        }
                    }
                }
            }

            Flickable{
                id: modelFlickable
                anchors {top: horizDivider2.bottom; bottom: parent.bottom; left: parent.left; right: parent.right}
                clip: true
                contentWidth: parent.width
                contentHeight: parent.height
                flickableDirection: Flickable.VerticalFlick

                Column {
                    id: nearbyDevicesList
                    width: parent.width
                    height: parent.height / 2

                    Repeater {
                        id: modelRepeater
                        model: nearbyDevicesModel

                        onCountChanged: {
                            modelFlickable.contentHeight = (count * 80)
                        }

                        delegate: DeviceDelegate {
                            id: availableBluetoothItem
                            width: nearbyDevicesList.width
                            deviceName: name
                            icon: model.icon
                            alias: model.alias
                            anchors {margins: 8}

                            onClicked: {
                                console.log("BUTTON CLICKED bubbled up")
                                nearbyDevicesModel.discover(false)
                                nearbyDevicesModel.pair(model.address)
                            }


                            Connections {
                                target: nearbyDevicesModel
                                onRequestConfirmation: {
                                    console.log("spawning request confirm dialog, device = " + device + " deviceName = " + deviceName)
                                    if(device != deviceName) return;

                                    dialogLoader.type = "confirmation"
                                    dialogLoader.device = device
                                    dialogLoader.code = code
                                    dialogLoader.sourceComponent = requestConfirmDialogComponent

                                }

                                onRequestPasskey: {
                                    console.log("spawning requestPasskeyDialog")
                                    if(device != deviceName) return;

                                    dialogLoader.type = "passkey"
                                    dialogLoader.device = device
                                    dialogLoader.sourceComponent = requestPasskeyDialogComponent

                                }

                                onRequestPidCode: {
                                    console.log("spawning requestPidCodeDialog")
                                    if(device != deviceName) return;


                                    dialogLoader.type = "pidcode"
                                    dialogLoader.device = device
                                    dialogLoader.legacyPairing = model.legacyPairing
                                    dialogLoader.sourceComponent = requestPidCodeDialogComponent

                                    console.log(device + " model legacyPairing: " + model.legacyPairing)
                                }
                            }
                        }
                    }
                }
            }


            Loader {
                id: dialogLoader
                anchors.fill: parent
                property string type: "NULL"
                property string device: ""
                property string code: ""
                property bool legacyPairing: false

                onLoaded: {
                    console.log("LOADER LOADED! type = " + type)
                    if (type === "confirmation" )
                    {
                        item.deviceName = device
                        item.key = code
                    }
                    else if (type === "passkey" )
                    {
                        item.deviceName = device
                    }
                    else if (type === "pidcode" )
                    {
                        item.deviceName = device
                        item.legacyPairing = legacyPairing
                    }
                }
            }

            Component {
                id: requestPasskeyDialogComponent
                RequestpasskeyDialog {
                    id: requestPasskeyDialog

                    onReplyRequestPasskey: {
                        dialogLoader.sourceComponent = undefined
                        nearbyDevicesModel.replyPasskey(reply)
                    }
                }
            }

            Component {
                id: requestPidCodeDialogComponent
                RequestPidCodeDialog {
                    id: requestPidCodeDialog
                    onReplyRequestPidCode: {
                        dialogLoader.sourceComponent = undefined
                        nearbyDevicesModel.replyRequestPidCode(reply)
                    }
                    onCancelRequest: {
                        dialogLoader.sourceComponent = undefined
                    }
                }
            }

            Component {
                id: requestConfirmDialogComponent
                RequestConfirmDialog {
                    id: requestConfirmDialog
                    onReplyRequestConfirmation: {
                        dialogLoader.sourceComponent = undefined
                        nearbyDevicesModel.replyRequestConfirmation(reply)
                    }
                }
            }
        }
    }
}

