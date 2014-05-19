/*
 *  Copyright (c) 2013, BLK Co., Ltd.  All right reserved.
 *
 *  This is unpublished proprietary information of BLK CO., Ltd and is
 *  protected as an unpublished work under applicable Copyright laws.
 *  The contents of this file may not be disclosed to third parties,
 *  copied or duplicated in any form, in whole or in part, without the
 *  prior written permission of BLK Co., Ltd.
 *
 *  $Id$
 *
 */

/**
 * @file The implementation of main window for HifuSys
 *
 * @author zhangshengfa
 * @date 2014-01-19
 */

#include "DisplayWindow.h"
#include "MonitorWindow.h"
#include "PlanWindow.h"
#include "MainWindow.h"

#include <QtGui>
#include <QSizePolicy>
#include <cstdlib>
#include <string>
#include <logger/Logger.h>
#include <config/IniParser.h>
#include <config/Config.h>
#include <config/IEnvConfig.h>

using namespace hifu;
using namespace hifu::logger;

MainWindow::MainWindow():
    m_mr_state(QAbstractSocket::UnconnectedState),
    m_hifu_state(QAbstractSocket::UnconnectedState)
{
    /* Create actions */
    createActions();

    /* Create menus */
    createMenus();

    /* Create toolbars */
    createToolBars();

    /* Create statusbar */
    createStatusBar();

    /* Create PlanWindow */
    m_plan_window = new PlanWindow(this);

    /* Create MonitorWindow */
    m_monitor_window = new MonitorWindow(this);

    /*
    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setWidget(m_monitor_window);
    scroll_area->viewport()->setAutoFillBackground(true);
    scroll_area->setWidgetResizable(true);
    */

    m_stacked_widget = new QStackedWidget(this);
    m_stacked_widget->addWidget(m_monitor_window);
    m_stacked_widget->addWidget(m_plan_window->getScrollArea());
    setCentralWidget(m_stacked_widget);

    /* Create dock widget */
    createDockWidgets();

    /* connect */
    connect(m_open_action, SIGNAL(triggered()), this, SLOT(on_open_action_triggered()));
    connect(m_monitor_action, SIGNAL(triggered()), this, SLOT(monitor()));
    connect(m_monitor_action, SIGNAL(triggered()), m_plan_window, SLOT(monitor()));
    connect(m_monitor_window, SIGNAL(imageSelectedSignal(DicomInstance *)), this, SLOT(plan(DicomInstance *)));
    connect(this, SIGNAL(planSignal(DicomInstance *)), m_plan_window, SLOT(setDicomImage(DicomInstance *)));

    /* Thread and TCP client */
    m_mr_thread = new ThreadManager();
    m_mr_client = new TcpClient();

    /* Set icon */
    setWindowIcon(QIcon(":/resource/images/hifusys.png"));
}

MainWindow::~MainWindow()
{
    m_mr_thread->wait();

    /*
    delete m_mr_thread;
    delete m_mr_client;
    */
}

void MainWindow::createActions()
{
    m_new_action = new QAction(tr("&New"), this);
    m_new_action->setIcon(QIcon(":/resource/images/new.png"));
    m_new_action->setShortcut(QKeySequence::New);
    m_new_action->setStatusTip(tr("Create a new patient treatment"));

    m_open_action = new QAction(tr("&Open..."), this);
    m_open_action->setIcon(QIcon(":/resource/images/open.png"));
    m_open_action->setShortcut(QKeySequence::Open);
    m_open_action->setStatusTip(tr("Open a exisiting patient treatment"));

    m_save_action = new QAction(tr("&Save"), this);
    m_save_action->setIcon(QIcon(":/resource/images/save.png"));
    m_save_action->setShortcut(QKeySequence::Save);
    m_save_action->setStatusTip(tr("Save current patient treatment to disk"));

    m_save_as_action = new QAction(tr("Save &As..."), this);
    m_save_as_action->setShortcut(QKeySequence::SaveAs);
    m_save_as_action->setStatusTip(tr("Save current patient treatment as new name"));

    m_print_action = new QAction(tr("&Print"), this);
    m_print_action->setIcon(QIcon(":/resource/images/print.png"));
    m_print_action->setShortcut(QKeySequence::Print);
    m_print_action->setStatusTip(tr("Print current patient treatment information"));

    m_print_preview_action = new QAction(tr("Print Pre&view"), this);
    m_print_preview_action->setStatusTip(tr("Preview the current patient information before print"));
   
    m_exit_action = new QAction(tr("E&xit"), this);
    m_exit_action->setShortcut(tr("Ctrl+Q"));
    m_exit_action->setStatusTip(tr("Logout and exit the HifuSys"));
    connect(m_exit_action, SIGNAL(triggered()), this, SLOT(close()));

    m_position_action = new QAction(tr("Po&sition"), this);
    m_position_action->setIcon(QIcon(":/resource/images/position.png"));
    m_position_action->setStatusTip(tr("Position the HIFU probe in 3D coordinate"));

    m_plan_action = new QAction(tr("&Plan"), this);
    m_plan_action->setIcon(QIcon(":/resource/images/plan.png"));
    m_plan_action->setStatusTip(tr("Make a treatment plan"));

    m_treat_action = new QAction(tr("&Treat"), this);
    m_treat_action->setIcon(QIcon(":/resource/images/treat.png"));
    m_treat_action->setStatusTip(tr("Control the treatment"));

    m_review_action = new QAction(tr("&Review"), this);
    m_review_action->setIcon(QIcon(":/resource/images/review.png"));
    m_review_action->setStatusTip(tr("Review after treatment"));

    m_connect_mr_action = new QAction(tr("&Connect"), this);
    m_connect_mr_action->setIcon(QIcon(":/resource/images/mr_connect.png"));
    m_connect_mr_action->setStatusTip(tr("Connect to MR"));
    
    m_disconnect_mr_action = new QAction(tr("&Disconnect"), this);
    m_disconnect_mr_action->setIcon(QIcon(":/resource/images/mr_disconnect.png"));
    m_disconnect_mr_action->setStatusTip(tr("Disconnect to MR"));

    m_transfer_action = new QAction(tr("&Transfer"), this);
    m_transfer_action->setIcon(QIcon(":/resource/images/transfer.png"));
    m_transfer_action->setStatusTip(tr("Download DICOM images from MR"));

    m_connect_hifu_action = new QAction(tr("&Connect"), this);
    m_connect_hifu_action->setIcon(QIcon(":/resource/images/hifu_connect.png"));
    m_connect_hifu_action->setStatusTip(tr("Connect to HIFU"));
    
    m_disconnect_hifu_action = new QAction(tr("&Disconnect"), this);
    m_disconnect_hifu_action->setIcon(QIcon(":/resource/images/hifu_disconnect.png"));
    m_disconnect_hifu_action->setStatusTip(tr("Disconnect to HIFU"));

    m_monitor_action = new QAction(tr("&Monitor"), this);
    m_monitor_action->setIcon(QIcon(":/resource/images/monitor.png"));
    m_monitor_action->setStatusTip(tr("Monitor the current curing"));

    m_show_main_toolbar_action = new QAction(tr("Show &Main ToolBar"), this);
    m_show_main_toolbar_action->setCheckable(true);
    m_show_main_toolbar_action->setChecked(true);
    m_show_main_toolbar_action->setStatusTip(tr("Show main toolbar or not"));

    m_show_statusbar_action = new QAction(tr("&StatusBar"), this);
    m_show_statusbar_action->setCheckable(true);
    m_show_statusbar_action->setChecked(true);
    m_show_statusbar_action->setStatusTip(tr("Show statusbar or not"));

    m_fullscreen_action = new QAction(tr("&Full Screen"), this);
    m_fullscreen_action->setCheckable(true);
    m_fullscreen_action->setChecked(false);
    m_fullscreen_action->setStatusTip(tr("Show full screen or not"));

    m_manual_action = new QAction(tr("&Manual"), this);
    m_manual_action->setStatusTip(tr("Show the manual of HifuSys"));

    m_about_hifusys_action = new QAction(tr("&About HifuSys"), this);
    m_about_hifusys_action->setStatusTip(tr("Show the HifuSys's About box"));

    m_start_treat_action = new QAction(tr("&Start Treat"), this);
    m_start_treat_action->setIcon(QIcon(":/resource/images/start.png"));
    m_start_treat_action->setStatusTip(tr("Start to treat"));

    m_stop_treat_action = new QAction(tr("S&top Treat"), this);
    m_stop_treat_action->setIcon(QIcon(":/resource/images/stop.png"));
    m_stop_treat_action->setStatusTip(tr("Stop to treat"));

    m_pause_treat_action = new QAction(tr("&Pause Treat"), this);
    m_pause_treat_action->setIcon(QIcon(":/resource/images/pause.png"));
    m_pause_treat_action->setStatusTip(tr("Treatment pause"));

    m_restart_treat_action = new QAction(tr("&Restart Treat"), this);
    m_restart_treat_action->setIcon(QIcon(":/resource/images/restart.png"));
    m_restart_treat_action->setStatusTip(tr("Restart to treat"));

    DLOG("create actions success");
}

void MainWindow::createMenus()
{
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_file_menu->addAction(m_new_action);
    m_file_menu->addAction(m_open_action);
    m_file_menu->addAction(m_save_action);
    m_file_menu->addAction(m_save_as_action);
    m_file_menu->addSeparator();
    m_file_menu->addAction(m_print_action);
    m_file_menu->addAction(m_print_preview_action);
    m_file_menu->addSeparator();
    m_file_menu->addAction(m_exit_action);

    m_therapy_menu = menuBar()->addMenu(tr("&Therapy"));
    m_therapy_menu->addAction(m_position_action);
    m_therapy_menu->addAction(m_plan_action);
    m_therapy_menu->addAction(m_treat_action);
    m_therapy_menu->addAction(m_review_action);
    m_therapy_menu->addAction(m_monitor_action);

    m_mr_menu = menuBar()->addMenu(tr("&MR"));
    m_mr_menu->addAction(m_connect_mr_action);
    m_mr_menu->addAction(m_disconnect_mr_action);
    m_mr_menu->addSeparator();
    m_mr_menu->addAction(m_transfer_action);

    m_hifu_menu = menuBar()->addMenu(tr("&HIFU"));
    m_hifu_menu->addAction(m_connect_hifu_action);
    m_hifu_menu->addAction(m_disconnect_hifu_action);

    m_view_menu = menuBar()->addMenu(tr("&View"));
    m_view_menu->addAction(m_fullscreen_action);
    m_view_menu->addSeparator();

    m_help_menu = menuBar()->addMenu(tr("&Help"));
    m_help_menu->addAction(m_manual_action);
    m_help_menu->addAction(m_about_hifusys_action);
}

void MainWindow::createToolBars()
{
    m_main_toolbar = addToolBar(tr("&Main ToolBar"));
    m_main_toolbar->setIconSize(QSize(32, 32));
    m_view_menu->addAction(m_main_toolbar->toggleViewAction());

    m_main_toolbar->addAction(m_new_action);
    m_main_toolbar->addAction(m_open_action);
    m_main_toolbar->addAction(m_save_action);
    m_main_toolbar->addAction(m_print_action);
    m_main_toolbar->addSeparator();

    m_main_toolbar->addAction(m_position_action);
    m_main_toolbar->addAction(m_plan_action);
    m_main_toolbar->addAction(m_treat_action);
    m_main_toolbar->addAction(m_monitor_action);
    m_main_toolbar->addAction(m_review_action);
    m_main_toolbar->addSeparator();

    m_main_toolbar->addAction(m_start_treat_action);
    m_main_toolbar->addAction(m_stop_treat_action);
    m_main_toolbar->addAction(m_pause_treat_action);
    m_main_toolbar->addAction(m_restart_treat_action);
    m_main_toolbar->addSeparator();

    m_main_toolbar->addAction(m_connect_mr_action);
    m_main_toolbar->addAction(m_disconnect_mr_action);
    m_main_toolbar->addAction(m_transfer_action);
    m_main_toolbar->addSeparator();

    m_main_toolbar->addAction(m_connect_hifu_action);
    m_main_toolbar->addAction(m_disconnect_hifu_action);
}

void MainWindow::createStatusBar()
{
    m_statusbar = statusBar();

    m_mr_state_label = new QLabel(" MR: host lookup... ");
    m_mr_state_label->setAlignment(Qt::AlignCenter);
    m_mr_state_label->setMinimumSize(m_mr_state_label->sizeHint());
    m_statusbar->addPermanentWidget(m_mr_state_label, 0);

    m_hifu_state_label = new QLabel(" HIFU: host lookup... ");
    m_hifu_state_label->setAlignment(Qt::AlignCenter);
    m_hifu_state_label->setMinimumSize(m_hifu_state_label->sizeHint());
    m_statusbar->addPermanentWidget(m_hifu_state_label, 0);

    updateStatusBar();
}

void MainWindow::createDockWidgets()
{
    QDockWidget *dock = new QDockWidget(tr("Image List"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_list_widget = new QListWidget(dock);
    m_list_widget->addItems(QStringList()
            << " Copyright (c) 2013, BLK Co., Ltd.  All right reserved. "
            << ""
            << " This is unpublished proprietary information of BLK CO., Ltd and is"
            << " protected as an unpublished work under applicable Copyright laws."
            << " The contents of this file may not be disclosed to third parties,"
            << " copied or duplicated in any form, in whole or in part, without the "
            << " prior written permission of BLK Co., Ltd.");
    dock->setWidget(m_list_widget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Patient List"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_list_widget = new QListWidget(dock);
    m_list_widget->addItems(QStringList()
            << " Copyright (c) 2013, BLK Co., Ltd.  All right reserved. "
            << ""
            << " This is unpublished proprietary information of BLK CO., Ltd and is"
            << " protected as an unpublished work under applicable Copyright laws."
            << " The contents of this file may not be disclosed to third parties,"
            << " copied or duplicated in any form, in whole or in part, without the "
            << " prior written permission of BLK Co., Ltd.");
    dock->setWidget(m_list_widget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    m_view_menu->addSeparator();
    m_view_menu->addAction(dock->toggleViewAction());
 
    dock = new QDockWidget(tr("Table"), this);
    m_table_widget = new QTableWidget(20, 2, dock);
    m_table_widget->setColumnWidth(1, 200);
    m_table_widget->setHorizontalHeaderLabels(QStringList()
            << "ID"
            << "Path");
    QTableWidgetItem *item = NULL;
    QString str;
    for (int i = 0; i < 20; i++) {
        str = tr("p0000%1").arg(i + 1);
        item = new QTableWidgetItem(str);
        m_table_widget->setItem(i, 0, item);

        item = new QTableWidgetItem(tr("/home/hifusys/images/%1/%2").arg(str).arg(i + 1));
        m_table_widget->setItem(i, 1, item);
    }
    dock->setWidget(m_table_widget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    m_view_menu->addAction(dock->toggleViewAction());

    /*
    dock = new QDockWidget(tr("display"), this);
    QScrollArea *scroll_area = new QScrollArea(dock);
    m_display_window = new DisplayWindow();
    scroll_area->setWidget(m_display_window);
    scroll_area->viewport()->setBackgroundRole(QPalette::Dark);
    scroll_area->viewport()->setAutoFillBackground(true);
    scroll_area->setWidgetResizable(true);
    scroll_area->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    // scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_display_window->load("../dicom");
    scroll_area->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    dock->setWidget(scroll_area);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    */
}

bool MainWindow::initialize()
{
    /* Load config first */
    if (!loadConfig()) {
        ELOG("loadConfig() failed");
        return false;
    }

    /* Initialize MR thread */
    if (!m_mr_thread->initialize("mr")) {
        ELOG("initialize() failed: name[mr]");
        return false;
    }

    /* Initialize MR TCP client */
    if (!m_mr_client->initialize(m_mr_ip, m_mr_port.toUShort())) {
        ELOG("initialize() failed: ip[%s], port[%s]", TO_PCHAR(m_mr_ip), TO_PCHAR(m_mr_port));
        return false;
    }

    /* Start thread */
    m_mr_thread->start();

    /* Move MR client to thread */
    m_mr_client->moveToThread(m_mr_thread);

    /* Initialize connections */
    if (!connect(m_connect_mr_action, SIGNAL(triggered()), this, SLOT(on_connect_mr_action_triggered()))) {
        ELOG("connect() failed: SIGNAL[triggered()], SLOT[on_connect_mr_action_triggered()]");
        return false;
    }

    if (!connect(this, SIGNAL(connectToMR()), m_mr_client, SLOT(connect()))) {
        ELOG("connect() failed: SIGNAL[connectToMR()], SLOT[connect()]");
        return false;
    }

    if (!connect(m_disconnect_mr_action, SIGNAL(triggered()), this, SLOT(on_disconnect_mr_action_triggered()))) {
        ELOG("connect() failed: SIGNAL[triggered()], SLOT[on_disconnect_mr_action_triggered()]");
        return false;
    }

    if (!connect(this, SIGNAL(disconnectFromMR()), m_mr_client, SLOT(disconnect()))) {
        ELOG("connect() failed: SIGNAL[disconnectFromMR()], SLOT[disconnect()]");
        return false;
    }

    if (!connect(m_mr_client, SIGNAL(connected()), this, SLOT(connectedToMR()))) {
        ELOG("connect() failed: SIGNAL[connected()], SLOT[connectedToMR()]");
        return false;
    }

    if (!connect(m_mr_client, SIGNAL(disconnected()), this, SLOT(disconnectedFromMR()))) {
        ELOG("connect() failed: SIGNAL[disconnected()], SLOT[disconnectedFromMR()]");
        return false;
    }

    qRegisterMetaType<QAbstractSocket::SocketState>();
    if (!connect(m_mr_client, SIGNAL(error(QAbstractSocket::SocketState, QString)), this, SLOT(errorFromMR(QAbstractSocket::SocketState, QString)))) {
        ELOG("connect() failed: SIGNAL[error()], SLOT[errorFromMR()]");
        return false;
    }

    if (!connect(this, SIGNAL(closeMRConnection()), m_mr_client, SLOT(close()))) {
        ELOG("connect() failed: SIGNAL[closeMRConnection], SLOT[close()]");
        return false;
    }

    return true;
}

void MainWindow::plan(DicomInstance *dcm_inst)
{
    m_stacked_widget->setCurrentIndex(1);
    emit planSignal(dcm_inst);
}

void MainWindow::monitor()
{
    m_stacked_widget->setCurrentIndex(0);
}

bool MainWindow::loadConfig()
{
    std::string file = IEnvConfig::instance()->getRootPath() + "/etc/" + HIFUSYS_CONFIG_FILE;
    tptree ptree;
    if (!config::iniparser::read(ptree, file)) {
        ELOG("config::read() failed: file[%s]", file.c_str());
        return false;
    }

    /* IP */
    std::string value;
    if (!config::get(ptree, "MR.IP", value)) {
        ELOG("get() failed: key[MR.IP]");
        return false;
    }
    DLOG("get() success: key[MR.IP], value[%s]", value.c_str());
    m_mr_ip = value.c_str();

    /* Port */
    if (!config::get(ptree, "MR.Port", value)) {
        ELOG("get() failed: key[MR.Port]");
        return false;
    }
    DLOG("get() success: key[MR.Port], value[%s]", value.c_str());
    m_mr_port = value.c_str();

    /* Path */
    if (!config::get(ptree, "MR.Path", value)) {
        ELOG("get() failed: key[MR.Path]");
        return false;
    }
    DLOG("get() success: key[MR.Path], value[%s]", value.c_str());
    m_mr_path = value.c_str();

    return true;
}

void MainWindow::connectedToMR()
{
    DLOG("connect() to MR success");

    m_mr_state = QAbstractSocket::ConnectedState;
    updateStatusBar();
}

void MainWindow::disconnectedFromMR()
{
    DLOG("disconncet() to MR success");

    m_mr_state = QAbstractSocket::UnconnectedState;
    updateStatusBar();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closeMRConnection();

    QMainWindow::closeEvent(event);
}

const QString& MainWindow::connectStateString(QAbstractSocket::SocketState state)
{
    static QString state_desc[] = {
        "unconnected",
        "host lookup...",
        "connecting...",
        "connected",
        "bound",
        "listening...",
        "closing..."
    };

    return state_desc[state];
}

void MainWindow::updateStatusBar()
{
    m_mr_state_label->setText(QString("MR: ") + connectStateString(m_mr_state));
    m_hifu_state_label->setText(QString("HIFU: ") + connectStateString(m_hifu_state));
}

void MainWindow::on_connect_mr_action_triggered()
{
    m_mr_state = QAbstractSocket::ConnectingState;
    updateStatusBar();

    emit connectToMR();
}

void MainWindow::on_disconnect_mr_action_triggered()
{
    m_mr_state = QAbstractSocket::ClosingState;
    updateStatusBar();

    emit disconnectFromMR();
}

void MainWindow::on_open_action_triggered()
{
    m_monitor_window->load();
}

void MainWindow::errorFromMR(QAbstractSocket::SocketState state, QString err_msg)
{
    ELOG("Socket error from MR: %s", TO_PCHAR(err_msg));

    m_mr_state = state;
    updateStatusBar();
}
