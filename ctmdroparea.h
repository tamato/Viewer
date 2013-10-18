#ifndef CTMDROPAREA_H
#define CTMDROPAREA_H

#include <QQuickItem>
/**
    Object that will except files dropped onto the app
*/
class CTMDropArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool acceptingDrops READ isAcceptingDrops WRITE setAcceptingDrops NOTIFY acceptingDropsChanged)

public:
    explicit CTMDropArea(QQuickItem *parent = 0);
    bool isAcceptingDrops() const;
    void setAcceptingDrops(bool accepting);

signals:
    void textDrop(QString text);
    void acceptingDropsChanged();

public slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    bool AcceptingDrops;
};

#endif // CTMDROPAREA_H
