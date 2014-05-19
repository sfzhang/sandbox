#ifndef _TESTWIDGET_H_
#define _TESTWIDGET_H_

#include <QWidget>

class QToolBar;
class QAction;
class QMainWindow;

class TestWidget: public QWidget {

    Q_OBJECT

public:

    TestWidget(QMainWindow *parent = NULL);

private:

    void createActions(QMainWindow *parent);

    void createToolBar(QMainWindow *parent);

private:

    QAction *m_cursor_action;
    QAction *m_add_action;

    QToolBar *m_test_toolbar;


};

#endif /* !_TESTWIDGET_H_ */
