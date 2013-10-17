#ifndef VIEWER_H
#define VIEWER_H

#include <QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class viewer : public QQuickItem
{
    Q_OBJECT
    //Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
public:
    explicit viewer(QQuickItem *parent = 0);
    
signals:
    
public slots:
    void paint();
    void cleanup();
    void sync();

private:
    QOpenGLShaderProgram *m_program;

    qreal m_t;
    qreal m_thread_t;

};

#endif // VIEWER_H
