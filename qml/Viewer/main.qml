import QtQuick 2.0

Rectangle {
    width: 360
    height: 360

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    DropArea {
        anchors.fill: parent
        onDropped: console.log("dropped")
        onEntered: console.log("entered")
    }


    Rectangle {
        width: 30
        height: 30
        x: 30
        y: 30
        color: "red"

        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: 15
        Drag.hotSpot.y: 15
        MouseArea {
            id: dragArea
            anchors.fill:parent
            drag.target:parent
            onReleased:Drag.drop()
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
