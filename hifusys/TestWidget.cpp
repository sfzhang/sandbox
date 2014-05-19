#include <QtGui>

#include "TestWidget.h"

TestWidget::TestWidget(QMainWindow *parent): QWidget(parent)
{
    createActions(parent);
    createToolBar(parent);
    parent->setCentralWidget(this);
}

void TestWidget::createActions(QMainWindow *parent)
{
    m_cursor_action = new QAction(tr("&Cursor"), parent);
    m_cursor_action->setIcon(QIcon(tr(":/resource/images/cursor.png")));

    m_add_action = new QAction(tr("&Add"), parent);
    m_add_action->setIcon(QIcon(tr(":/resource/images/add.png")));
    m_add_action->setCheckable(true);
}

void TestWidget::createToolBar(QMainWindow *parent)
{
    m_test_toolbar = new QToolBar(tr("&test"));
    m_test_toolbar->addAction(m_cursor_action);
    m_test_toolbar->addAction(m_add_action);
    m_test_toolbar->setAllowedAreas(Qt::LeftToolBarArea);
    m_test_toolbar->setIconSize(QSize(24, 24));
    m_test_toolbar->setOrientation(Qt::Vertical);
    parent->addToolBar(Qt::LeftToolBarArea, m_test_toolbar);
}
