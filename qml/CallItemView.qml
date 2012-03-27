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
  anchors.fill: parent
  property variant call: null

  state: 'disconnected'
  onStateChanged: {
     console.log("*** STATE IS now >> " + root.state);
  }

  onCallChanged: {
    if(call) {
      largeView.call = call;
      console.log("*** QML *** :: CALL ITEM CHANGED, STATE: " + call.state);
      root.state = call.state;

      if (call.stateChanged)
      {
      call.stateChanged.connect(function(state) {
       console.log("*** QML *** :: CALL ITEM STATE CHANGED: " + state);
       console.log("");

        root.state = state;
     });
    }
    else
    {
	console.log("*** call.stateChanged doesn't exist.. setting state to disconnect");
	//	root.state = 'disconnected';
    }
    }
    else
	console.log("*** call doesn't exist.. setting to disconnect");
	//root.state = 'disconnected';
  }

  states {
    State {
      name: 'active'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'held'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'dialing'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'alerting'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'incoming'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'waiting'
      PropertyChanges {target: root; visible: true}
    }

    State {
      name: 'disconnected'
      PropertyChanges {target: root; visible: false}
    }
  }

  Rectangle
  {
    id: background
    anchors {top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom; topMargin: parent.height / 4; bottomMargin: parent.height / 5}

    gradient: Gradient {
      GradientStop {position: 0.0; color: '#4f4f4f'}
      GradientStop {position: 1.0; color: '#000000'}
    }
   
    CallItemViewLarge
    {
     id: largeView
     call: root.call //parent.call
     state: root.state //parent.state
    }
}
}

