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
 * @file The implementation of treatment plan window
 *
 * @author zhangshengfa
 * @date 2014-01-22
 */

#include <QtGui>
#include <cassert>
#include <logger/Logger.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "PlanWindow.h"
#include "qdicomimage.h"

using namespace hifu::logger;

PlanWindow::PlanWindow(QWidget *parent): m_dcm_inst(NULL)
{
    /* Create actions */
    createActions();

    /* Create toolbars */
    // createToolBars();

    m_prev_point = QPoint(-1, -1);

    m_orig_size = QSize(-1, -1);

    // setParent(owner);
    // owner->addToolBar(Qt::LeftToolBarArea, m_plan_toolbar);

    m_scroll_area = new QScrollArea(parent);
    m_scroll_area->setWidget(this);
    // m_scroll_area->viewport()->setBackgroundColor(QColor(90, 90, 90));
    m_scroll_area->viewport()->setAutoFillBackground(true);
    m_scroll_area->setWidgetResizable(true);
    // m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

bool PlanWindow::load(const std::string &dir)
{
	/* read the directory */
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if (NULL == (dp = opendir(dir.c_str()))) {
		ELOG("opendir() failed: dir[%s], errno[%d]", dir.c_str(), errno);
		return false;
	}

	char file[1024] = {0};
	struct stat st;
	memset(&st, 0, sizeof(st));
	while (dirp = readdir(dp)) {
		snprintf(file, sizeof(file), "%s/%s", dir.c_str(), dirp->d_name);
		DLOG("file[%s]", file);

		if (stat(file, &st) != 0) {
			ELOG("stat() failed: file[%s], errno[%d]", file, errno);
			continue;
		}

		/* ignore current directory(.) and parent directory(..) */
		if (0 == strncmp(dirp->d_name, ".", 1)) {
			DLOG("current directory[.] ignored");
			continue;
		}

		if (0 == strcmp(dirp->d_name, "..")) {
			DLOG("parent directory[..] ignored");
			continue;
		}

        if (S_ISDIR(st.st_mode)) {
            if (!load(file)) {
                ELOG("load() failed: file[%s]", file);
                return false;
            }
        }

		if (!S_ISREG(st.st_mode)) {
            DLOG("file[%s] is not file", file);
            continue;
        }
        
        DicomInstance *dcm_inst = new DicomInstance();
        if (!dcm_inst->load(file)) {
            ELOG("load() failed: %s", file);
            return false;
        }

        dcm_inst->setMinMaxWindow();
    }

	closedir(dp);

	return true;
}

void PlanWindow::createActions()
{
    m_cursor_action = new QAction(tr("&New"), this);
    m_cursor_action->setIcon(QIcon(":/resource/images/cursor.png"));
    m_cursor_action->setStatusTip(tr("Become cursor"));

    m_clear_action = new QAction(tr("&Clear"), this);
    m_clear_action->setIcon(QIcon(":/resource/images/clear.png"));
    m_clear_action->setStatusTip(tr("Clear all planning"));

    m_text_action = new QAction(tr("&Text"), this);
    m_text_action->setIcon(QIcon(":/resource/images/text.png"));
    m_text_action->setCheckable(true);
    m_text_action->setStatusTip(tr("Show text or not"));

    m_ruler_action = new QAction(tr("&Ruler"), this);
    m_ruler_action->setIcon(QIcon(":/resource/images/ruler.png"));
    m_ruler_action->setCheckable(true);
    m_ruler_action->setStatusTip(tr("Show ruler or not"));

    m_window_action = new QAction(tr("&Window"), this);
    m_window_action->setIcon(QIcon(":/resource/images/window.png"));
    m_window_action->setStatusTip(tr("Windowing the image"));

    m_zoom_in_action = new QAction(tr("Zoom &In"), this);
    m_zoom_in_action->setIcon(QIcon(":/resource/images/zoom_in.png"));
    m_zoom_in_action->setStatusTip(tr("Zoom in image"));

    m_zoom_out_action = new QAction(tr("Zoom &Out"), this);
    m_zoom_out_action->setIcon(QIcon(":/resource/images/zoom_out.png"));
    m_zoom_out_action->setStatusTip(tr("Zoom out image"));

    m_draw_action = new QAction(tr("&Draw"), this);
    m_draw_action->setIcon(QIcon(":/resource/images/draw.png"));
    m_draw_action->setStatusTip(tr("Draw image"));

    m_line_action = new QAction(tr("&Line"), this);
    m_line_action->setIcon(QIcon(":/resource/images/line.png"));
    m_line_action->setStatusTip(tr("Draw line"));

    m_rectangle_action = new QAction(tr("&Rectange"), this);
    m_rectangle_action->setIcon(QIcon(":/resource/images/rectangle.png"));
    m_rectangle_action->setStatusTip(tr("Draw rectangel"));

    m_circle_action = new QAction(tr("&Circle"), this);
    m_circle_action->setIcon(QIcon(":/resource/images/circle.png"));
    m_circle_action->setStatusTip(tr("Draw circle"));

    m_add_point_action = new QAction(tr("&Add point"), this);
    m_add_point_action->setIcon(QIcon(":/resource/images/add_point.png"));
    m_add_point_action->setStatusTip(tr("Add points to treatment"));

    m_delete_point_action = new QAction(tr("&Delete point"), this);
    m_delete_point_action->setIcon(QIcon(":/resource/images/delete_point.png"));
    m_delete_point_action->setStatusTip(tr("Delete points from treatment"));

    m_move_point_action = new QAction(tr("&Move point"), this);
    m_move_point_action->setIcon(QIcon(":/resource/images/move_point.png"));
    m_move_point_action->setStatusTip(tr("Move points in treatment"));

    m_in_action = new QAction(tr("&In"), this);
    m_in_action->setIcon(QIcon(":/resource/images/in.png"));
    m_in_action->setStatusTip(tr("in"));

    m_out_action = new QAction(tr("&Out"), this);
    m_out_action->setIcon(QIcon(":/resource/images/out.png"));
    m_out_action->setStatusTip(tr("out"));

    m_clock_wise_action = new QAction(tr("&Clock wise"), this);
    m_clock_wise_action->setIcon(QIcon(":/resource/images/clock_wise.png"));
    m_clock_wise_action->setStatusTip(tr("Clock wise"));

    m_anticlock_wise_action = new QAction(tr("&Anticlock wise"), this);
    m_anticlock_wise_action->setIcon(QIcon(":/resource/images/anticlock_wise.png"));
    m_anticlock_wise_action->setStatusTip(tr("Anticlock wise"));

    m_generate_action = new QAction(tr("&Generate"), this);
    m_generate_action->setIcon(QIcon(":/resource/images/generate.png"));
    m_generate_action->setStatusTip(tr("Generate treatment points"));

    DLOG("create actions success");
}

void PlanWindow::createToolBars()
{
    m_plan_toolbar = new QToolBar(tr("&Plan toolbar"));
    m_plan_toolbar->setIconSize(QSize(28, 28));
    m_plan_toolbar->addAction(m_cursor_action);
    m_plan_toolbar->addAction(m_clear_action);
    m_plan_toolbar->addAction(m_text_action);
    m_plan_toolbar->addAction(m_ruler_action);
    m_plan_toolbar->addSeparator();
    m_plan_toolbar->addAction(m_window_action);
    m_plan_toolbar->addAction(m_zoom_in_action);
    m_plan_toolbar->addAction(m_zoom_out_action);
    m_plan_toolbar->addSeparator();
    m_plan_toolbar->addAction(m_draw_action);
    m_plan_toolbar->addAction(m_line_action);
    m_plan_toolbar->addAction(m_rectangle_action);
    m_plan_toolbar->addAction(m_circle_action);
    m_plan_toolbar->addSeparator();
    m_plan_toolbar->addAction(m_add_point_action);
    m_plan_toolbar->addAction(m_delete_point_action);
    m_plan_toolbar->addAction(m_move_point_action);
    m_plan_toolbar->addSeparator();
    m_plan_toolbar->addAction(m_in_action);
    m_plan_toolbar->addAction(m_out_action);
    m_plan_toolbar->addAction(m_clock_wise_action);
    m_plan_toolbar->addAction(m_anticlock_wise_action);
    m_plan_toolbar->addAction(m_generate_action);
}

void PlanWindow::paintEvent(QPaintEvent *event)
{
    if (!m_dcm_inst) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int w = width();
    int h = height();

    unsigned long image_width = 256;
    unsigned long image_height = 256;
    if ((w / image_width) > (h / image_height)) {
        painter.drawImage(QRectF((w - h) / 2, 0, h, h), m_dcm_inst->getOutputData());
    }
    else {
        painter.drawImage(QRectF(0, (h - w) / 2, w, w), m_dcm_inst->getOutputData());
    }
}

void PlanWindow::mousePressEvent(QMouseEvent *event)
{
    m_prev_point = event->globalPos();
    update();
}

void PlanWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dcm_inst) {
        return;
    }

    if (event->buttons() & Qt::RightButton) {
	    double x = (event->globalX() - m_prev_point.x()) / 1.0;
	    double y = (event->globalY() - m_prev_point.y()) / 1.0;
	
        double center = 0;
        double width = 0;
        m_dcm_inst->getWindow(center, width);
	    m_dcm_inst->setWindow(center + x, width + y);

        update();
	}
    else if (event->buttons() & Qt::LeftButton) {
        int x = event->globalX() - m_prev_point.x();
	    int y = event->globalY() - m_prev_point.y();


        if (m_scroll_area) {
            int value = 0;

            if (m_scroll_area->horizontalScrollBar()->isVisible()) {
	            value = m_scroll_area->horizontalScrollBar()->value() - x;
	            if (value < m_scroll_area->horizontalScrollBar()->minimum()) {
	                value = m_scroll_area->horizontalScrollBar()->minimum();
	            }
	            else if (value > m_scroll_area->horizontalScrollBar()->maximum()) {
	                value = m_scroll_area->horizontalScrollBar()->maximum();
	            }
	            m_scroll_area->horizontalScrollBar()->setValue(value);
            }

            if (m_scroll_area->verticalScrollBar()->isVisible()) {
                DLOG("v: %d, dy: %d, pos[%d, %d], x[%d], y[%d], gx[%d], gy[%d]",
                     m_scroll_area->verticalScrollBar()->value(), y, event->pos().x(), event->pos().y(), event->x(), event->y(),
                     event->globalX(), event->globalY());

                value = m_scroll_area->verticalScrollBar()->value() - y;
	            if (value < m_scroll_area->verticalScrollBar()->minimum()) {
	                value = m_scroll_area->verticalScrollBar()->minimum();
	            }
	            else if (value > m_scroll_area->verticalScrollBar()->maximum()) {
	                value = m_scroll_area->verticalScrollBar()->maximum();
	            }
	            m_scroll_area->verticalScrollBar()->setValue(value);
            }
        }
    }

    m_prev_point = event->globalPos();
}

void PlanWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_prev_point = QPoint(-1, -1);
}

void PlanWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_dcm_inst) {
        return;
    }

    m_prev_point = QPoint(-1, -1);
    m_dcm_inst->setMinMaxWindow();
	setMinimumSize(m_orig_size);
	resize(m_orig_size);
	
    update();
}

void PlanWindow::resizeEvent(QResizeEvent *event)
{
    static int i = 0;
    DLOG("i[%d], w[%d], h[%d]", i++, event->size().width(), event->size().height());

    QWidget::resizeEvent(event);
}

void PlanWindow::wheelEvent(QWheelEvent *event)
{
    if (1) {
	    QSize s = size();
	    if (QSize(-1, -1) == m_orig_size) {
	        m_orig_size = s;
	        setMinimumSize(s);
	        DLOG("wheelEvent: w[%d], h[%d]", s.width(), s.height());
	    }
	
	    int degrees = event->delta() / 8;
	    int steps = degrees / 1;
	
	    int w = s.width() + steps;
	    int h = s.height() + steps;
	
	    if ((w < m_orig_size.width()) || (h < m_orig_size.height())) {
	        setMinimumSize(m_orig_size);
	        resize(m_orig_size);
	    }
	    else {
	        setMinimumSize(QSize(w, h));
	        resize(QSize(w, h));
	    }
    }
    else {
        QWidget::wheelEvent(event);
    }
}

void PlanWindow::setDicomImage(DicomInstance *dcm_inst)
{

    m_dcm_inst = dcm_inst;

    if (!m_dcm_inst) {
        return;
    }

    if (!m_dcm_inst->setMinMaxWindow()) {
        ELOG("setMinMaxWindow() failed");
        return;
    }

    m_prev_point = QPoint(-1, -1);
    m_orig_size = QSize(-1, -1);

    update();
}

void PlanWindow::monitor()
{
    if (m_dcm_inst) {
        m_dcm_inst->setMinMaxWindow();
        m_dcm_inst = NULL;
    }
}
