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
 * @file The interface of main window for HifuSys 
 *
 * @author zhangshengfa
 * @date 2014-01-19
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QString>
#include <QtNetwork/QTcpSocket>
#include <logger/Logger.h>

#include "TcpClient.h"
#include "ThreadManager.h"
#include "HifuSysDefs.h"

class QMenu;
class QLabel;
class QToolBar;
class QStatusBar;
class QAction;
class PlanWindow;
class QTableWidget;
class QListWidget;
class DisplayWindow;
class MonitorWindow;
class DicomInstance;
class QStackedWidget;

class MainWindow: public QMainWindow {

    Q_OBJECT

public:

    /**
     * @brief Default constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    MainWindow();

    /**
     * @brief Destruct constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    ~MainWindow();

    /**
     * @brief Initialize
     *
     * @param N/A
     *
     * @retval true Initialize success
     * @retval false Failed to initialize
     */
    bool initialize();

private:

    /**
     * @brief Load configure file
     *
     * @param N/A
     *
     * @retval true Load configure file successfully
     * @retval false Failed to load configure file
     */
    bool loadConfig();

    /**
     * @brief Create actions
     *
     * @param N/A
     *
     * @return N/A
     */
    void createActions();

    /**
     * @brief Create menus
     *
     * @param N/A
     *
     * @return N/A
     */
    void createMenus();

    /**
     * @brief Create toolbars
     *
     * @param N/A
     *
     * @return N/A
     */
    void createToolBars();

    /**
     * @brief Create statusbar
     *
     * @param N/A
     *
     * @return N/A
     */
    void createStatusBar();

    /**
     * @brief Create DockWidget
     *
     * @param N/A
     *
     * @return N/A
     */
    void createDockWidgets();

    /**
     * @brief Update statusbar
     *
     * @param N/A
     *
     * @return N/A
     */
    void updateStatusBar();

    /**
     * @brief Get connect state description
     *
     * @param[in] state The connect state
     *
     * @return The description of the given connect state
     */
    static const QString& connectStateString(QAbstractSocket::SocketState state);


protected:

    virtual void closeEvent(QCloseEvent *event);

signals:

    void planSignal(DicomInstance *dcm_inst);

    void connectToMR();
    void disconnectFromMR();

    void closeMRConnection();

private slots:

    void on_connect_mr_action_triggered();
    void on_disconnect_mr_action_triggered();
    void on_open_action_triggered();

public slots:

    void plan(DicomInstance *dcm_inst);
    void monitor();
    void connectedToMR();
    void disconnectedFromMR();
    void receiveFromMR() {};

    void errorFromMR(QAbstractSocket::SocketState state, QString err_msg);
 
private:

    /* Menus */
    QMenu *m_file_menu;                 /**< Menu for file */
    QMenu *m_therapy_menu;              /**< Menu for therapy */
    QMenu *m_mr_menu;                   /**< Menu for MR */
    QMenu *m_hifu_menu;                 /**< Menu for HIFU */
    QMenu *m_view_menu;                 /**< Menu for view */
    QMenu *m_help_menu;                 /**< Menu for help */

    /* ToolBars */
    QToolBar *m_main_toolbar;           /**< Main toolbar */
    QToolBar *m_file_toolbar;           /**< ToolBar for patient */
    QToolBar *m_therapy_toolbar;        /**< ToolBar for therapy */
    QToolBar *m_mr_toolbar;             /**< ToolBar for MR */
    QToolBar *m_hifu_toolbar;           /**< ToolBar for HIFU */
    QToolBar *m_treat_toolbar;          /**< ToolBar for treatment */

    /* StatusBar */
    QStatusBar *m_statusbar;            /**< StatusBar for main window */
    QLabel *m_mr_state_label;           /**< Label for MR state */
    QAbstractSocket::SocketState m_mr_state;    /**< MR connect state */
    QLabel *m_hifu_state_label;         /**< Label for HIFU state */
    QAbstractSocket::SocketState m_hifu_state;  /**< HIFU connect state */

    /* Actions for menu, main toolbars */
    QAction *m_new_action;              /**< Action for new patient */
    QAction *m_open_action;             /**< Action for open */
    QAction *m_save_action;             /**< Action for save */
    QAction *m_save_as_action;          /**< Action for save as */
    QAction *m_print_action;            /**< Action for print */
    QAction *m_print_preview_action;    /**< Action for print preview */
    QAction *m_exit_action;             /**< Action for exit */

    QAction *m_position_action;         /**< Action for position */
    QAction *m_plan_action;             /**< Action for plan */
    QAction *m_treat_action;            /**< Action for treatment */
    QAction *m_review_action;           /**< Action for review */

    QAction *m_connect_mr_action;       /**< Action for connect to MR */
    QAction *m_disconnect_mr_action;    /**< Action for disconnect to MR */
    QAction *m_transfer_action;         /**< Action for transfer with MR */

    QAction *m_connect_hifu_action;     /**< Action for connect to HIFU */
    QAction *m_disconnect_hifu_action;  /**< Action for disconnect to HIFU */
    QAction *m_monitor_action;          /**< Action for monitor curing */

    QAction *m_show_main_toolbar_action;/**< Action for show main toolbar */
    QAction *m_show_statusbar_action;   /**< Action for show main statusbar */
    QAction *m_fullscreen_action;       /**< Action for full screen */

    QAction *m_manual_action;           /**< Action for manual */
    QAction *m_about_hifusys_action;    /**< Action for HifuSys information */

    /* Actions for treatment */
    QAction *m_start_treat_action;
    QAction *m_stop_treat_action;
    QAction *m_pause_treat_action;
    QAction *m_restart_treat_action;

    /* PlanWindow */
    PlanWindow *m_plan_window;

    /* DockWidget */
    QListWidget *m_list_widget;
    QTableWidget *m_table_widget;
    
    /* DisplayWindow */
    DisplayWindow *m_display_window;

    /* MonitorWindow */
    MonitorWindow *m_monitor_window;

    /* Stack widget */
    QStackedWidget *m_stacked_widget;

    QString m_mr_ip;            /**< MR IP */
    QString m_mr_port;          /**< MR port */
    QString m_mr_path;          /**< MR DICOM image path */

    ThreadManager *m_mr_thread; /**< MR thread */
    TcpClient *m_mr_client;     /**< MR TCP client */

};

#endif /* !_MAINWINDOW_H_ */
