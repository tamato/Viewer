import QtQuick 2.0
import ctmdroparea 1.0

Rectangle {
    width: 360
    height: 360

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    CTMDropArea {
        x: 60; y: 60
        width: 120; height: 120
        onDropped: console.log("dropped")
//        onEntered: console.log("entered")

        Rectangle {
            color: "green"
            anchors.fill: parent
            visible: parent.containsDrag
        }
    }

    //http://stackoverflow.com/questions/7618307/drag-and-drop-in-qml
    Rectangle {
        width: 130; height: 130
        x: 130; y: 130
        color: "red"

        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: 15
        Drag.hotSpot.y: 15
        MouseArea {
            id: dragArea
            anchors.fill: parent
            onReleased: parent.Drag.drop()
            drag.target: parent
        }
    }

//    MouseArea {

//        id: dragArea
//        anchors.fill: parent
//        drag.target: parent
//        onReleased: Drag.drop()

//        onClicked: {
//            //Qt.quit();
//        }
//    }
}
