#ifndef VIEWER_H
#define VIEWER_H

#include <QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class Viewer : public QQuickItem
{
    Q_OBJECT

public:
    explicit Viewer(QQuickItem *parent = 0);

signals:

public slots:
    void paint();
    void cleanup();
    void sync();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLShaderProgram *Program;
};

#endif // VIEWER_H
