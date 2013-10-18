#include "ctmdroparea.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtDebug>

// http://qt-project.org/doc/qt-5.1/qtgui/dnd.html
// http://qt-project.org/doc/qt-5.1/qtgui/qdropevent.html

CTMDropArea::CTMDropArea(QQuickItem *parent) :
    QQuickItem(parent)
  , AcceptingDrops(true)
{
    setAcceptingDrops(AcceptingDrops);
}

bool CTMDropArea::isAcceptingDrops() const
{
    return AcceptingDrops;
}

void CTMDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void CTMDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
}

void CTMDropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void CTMDropArea::dropEvent(QDropEvent *event)
{
    emit textDrop(event->mimeData()->text());
    qDebug() << "Drop recieved: " << event->mimeData()->text();
    event->acceptProposedAction();
}

void CTMDropArea::setAcceptingDrops(bool accepting)
{
    if (accepting == AcceptingDrops)
                return;

    AcceptingDrops = accepting;
    setAcceptingDrops(AcceptingDrops);
    emit acceptingDropsChanged();
}
