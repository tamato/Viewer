#include "viewer.h"

viewer::viewer(QQuickItem *parent) :
    QQuickItem(parent)
  , m_program(0)
{
}

void viewer::paint()
{
}

void viewer::cleanup()
{
    if (m_program) {
        delete m_program;
        m_program = 0;
    }
}

void viewer::sync()
{
}
