#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "qtquick2applicationviewer.h"

#include "ctmdroparea.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // first arg: what to improt in qml file to get access to QML object.
    // fourth arg: QML object name.
    qmlRegisterType<CTMDropArea>("ctmdroparea", 1, 0, "CTMDropArea");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Viewer/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
