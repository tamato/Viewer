#include "ctmdroparea.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtDebug>

// http://qt-project.org/doc/qt-5.1/qtgui/dnd.html
// http://qt-project.org/doc/qt-5.1/qtgui/qdropevent.html
// https://qt-project.org/doc/qt-5.1/qtquick/qml-qtquick2-droparea.html

CTMDropArea::CTMDropArea(QQuickItem *parent) :
    QQuickItem(parent)
  , AcceptingDrops(false)
{
}

bool CTMDropArea::isAcceptingDrops() const
{
    return AcceptingDrops;
}

void CTMDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "Drag Event";
    event->acceptProposedAction();
}

void CTMDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
}

void CTMDropArea::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "Move Event";
    event->acceptProposedAction();
}

void CTMDropArea::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop recieved: " << event->mimeData()->text();
    emit textDrop(event->mimeData()->text());
    event->acceptProposedAction();
}

void CTMDropArea::setAcceptingDrops(bool accepting)
{
    if (accepting == AcceptingDrops)
                return;

    setFlag(QQuickItem::ItemAcceptsDrops, true);
    AcceptingDrops = accepting;
    emit acceptingDropsChanged();
}
