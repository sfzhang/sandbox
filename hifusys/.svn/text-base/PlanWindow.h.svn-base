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
 * @file The interface of treatment plan window
 *
 * @author zhangshengfa
 * @date 2014-01-19
 */

#ifndef _PLANWINDOW_H_
#define _PLANWINDOW_H_

#include <QWidget>
#include <vector>
#include "DicomInstance.h"

class QToolBar;
class QAction;
class QMainWindow;
class QGraphicsView;
class QImage;
class QScrollArea;

class PlanWindow: public QWidget {

    Q_OBJECT

public:

    /**
     * @brief Default constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    PlanWindow(QWidget *parent);

    /** @brief load dicom image */
    bool load(const std::string &dir);

    /** @brief Get scroll area */
    QScrollArea *getScrollArea()
    {
        return m_scroll_area;
    }

public slots:

    void setDicomImage(DicomInstance *dcm_inst);
    void monitor();
    
private:

    /**
     * @brief Create actions
     *
     * @param N/A
     *
     * @return N/A
     */
    void createActions();

    /**
     * @brief Create toolbars
     *
     * @param N/A
     *
     * @return N/A
     */
    void createToolBars();

protected:

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);

    void wheelEvent(QWheelEvent *event);

private:

    /* ToolBars */
    QToolBar *m_plan_toolbar;           /**< ToolBar for treatment plan */

    /* Actions for toolbars */
    QAction *m_cursor_action;           /**< Action for cursor */
    QAction *m_clear_action;            /**< Action for clear */
    QAction *m_text_action;             /**< Action for show text or not */
    QAction *m_ruler_action;            /**< Action for show ruler or not */
    QAction *m_window_action;           /**< Action for windowing */
    QAction *m_zoom_in_action;          /**< Action for zoom in */
    QAction *m_zoom_out_action;         /**< Action for zoom out */
    QAction *m_draw_action;             /**< Action for draw */
    QAction *m_line_action;             /**< Action fro draw line */
    QAction *m_rectangle_action;        /**< Action for draw rectangel */
    QAction *m_circle_action;           /**< Action for draw circle */
    QAction *m_add_point_action;        /**< Action for add treatment point */
    QAction *m_delete_point_action;     /**< Action for delete treatment point */
    QAction *m_move_point_action;       /**< Action for move treatment point */
    QAction *m_in_action;               /**< Action for in */
    QAction *m_out_action;              /**< Action for out */
    QAction *m_clock_wise_action;       /**< Action for clock_wise */
    QAction *m_anticlock_wise_action;   /**< Action for anticlock_wise */
    QAction *m_generate_action;         /**< Action for generate treatment points */

    DicomInstance *m_dcm_inst;

    double m_scale;

    QPoint m_prev_point;

    QSize m_orig_size;

    QScrollArea *m_scroll_area;

    QMainWindow *m_owner;

};

#endif /* !_PLANWINDOW_H_ */
