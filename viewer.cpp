#include "viewer.h"
#include <QtQuick/qquickwindow.h> // get access to window()
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtDebug>  // qDebug()

Viewer::Viewer(QQuickItem *parent) :
    QQuickItem(parent)
  , Program(0)
{
    qDebug() << "viewr ctor";
    setFlag(QQuickItem::ItemAcceptsDrops, true);
    connect(
        this, SIGNAL(windowChanged(QQuickWindow*)),
        this, SLOT(handleWindowChanged(QQuickWindow*))
        );
}

void Viewer::paint()
{
    glViewport(0, 0, window()->width(), window()->height());
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    qDebug() << "hello";
}

void Viewer::cleanup()
{
    if (Program) {
        delete Program;
        Program = 0;
    }
}

void Viewer::sync()
{
}

void Viewer::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "Drag Event";
    event->acceptProposedAction();
}

void Viewer::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "Move Event";
    event->acceptProposedAction();
}

void Viewer::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop recieved: " << event->mimeData()->text();
    event->acceptProposedAction();
}

void Viewer::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
