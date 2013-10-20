#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include "qtquick2applicationviewer.h"
#include <QtDebug>  // qDebug()
#include "viewer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // first arg: what to import in qml file to get access to QML object.
    // fourth arg: QML object name.
    qmlRegisterType<Viewer>("meshviewer", 1, 0, "MeshViewer");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Viewer/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
