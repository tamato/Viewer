import QtQuick 2.0
import meshviewer 1.0

MeshViewer {
    width: 360
    height: 360

    // http://qt-project.org/doc/qt-5.0/qtquick/qtquick-input-focus.html
    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) {
            Qt.quit()
            event.accepted = true
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
}
