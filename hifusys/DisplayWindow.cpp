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
 * @file The implementation of treatment display window
 *
 * @author zhangshengfa
 * @date 2014-02-24
 */

#include <QtGui>
#include <cassert>
#include <logger/Logger.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "DisplayWindow.h"
#include "qdicomimage.h"

using namespace hifu::logger;

bool DisplayWindow::load(const std::string &dir)
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
		if (0 == strcmp(dirp->d_name, ".")) {
			DLOG("current directory[.] ignored");
			continue;
		}

		if (0 == strcmp(dirp->d_name, "..")) {
			DLOG("parent directory[..] ignored");
			continue;
		}

		if (!S_ISREG(st.st_mode)) {
            DLOG("file[%s] is not file", file);
            continue;
        }
        qDebug("%s", file);
        
        QDicomImage *dicom_image = new QDicomImage(file);
        dicom_image->setMinMaxWindow();
        m_dicom_image_vector.push_back(dicom_image);
        m_image_vector.push_back(dicom_image->outputData());
    }

	closedir(dp);

    setMinimumSize(1 + 256 + 1, m_dicom_image_vector.size() * 256 + (m_dicom_image_vector.size() + 1));
    qDebug("load: w %d, h: %d", size().width(), size().height());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	return true;
}

QSize DisplayWindow::sizeHint() const
{
    if (m_dicom_image_vector.size() > 0) {
        QSize s = QSize(1 + 256 + 1 + qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent),
                        m_dicom_image_vector.size() * 256 + (m_dicom_image_vector.size() + 1));
        return s;
    }
    else {
        return QWidget::sizeHint();
    }
}

void DisplayWindow::paintEvent(QPaintEvent *event)
{
    if (0 == m_dicom_image_vector.size()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    for (int i = 0; i < m_dicom_image_vector.size(); i++) {
        painter.drawImage(QRect(1, (1 + i + i * 256), 256, 256), m_image_vector[i]);
    }

    if (m_selected_image_index >= 0) {
        painter.setPen(QColor(0xFF, 0x8C, 0x00));
        painter.drawLine(0, ((1 + 256) * m_selected_image_index), (1 + 256), ((1 + 256) * m_selected_image_index));
        painter.drawLine((1 + 256), ((1 + 256) * m_selected_image_index), (1 + 256), ((1 + 256) * (m_selected_image_index + 1)));
        painter.drawLine(0, ((1 + 256) * (m_selected_image_index + 1)), (1 + 256), ((1 + 256) * (m_selected_image_index + 1)));
        painter.drawLine(0, ((1 + 256) * (m_selected_image_index)), 0, ((1 + 256) * (m_selected_image_index + 1)));
    }
}

void DisplayWindow::mousePressEvent(QMouseEvent *event)
{
    if (0 == m_dicom_image_vector.size()) {
        return;
    }

    if (Qt::LeftButton == event->button()) {
        int x = event->x();
        int y = event->y();

        if ((x >= 1) && (x <= 256)) {
            m_selected_image_index = y / (1 + 256);
        }
        else {
            m_selected_image_index = -1;
        }

        qDebug("x: %d, y: %d, selected: %d", x, y, m_selected_image_index);
        update();

        if ((m_selected_image_index >= 0) && (m_selected_image_index < m_dicom_image_vector.size())) {
            emit imageSelectedSignal(m_dicom_image_vector[m_selected_image_index]);
        }
        else {
            emit imageSelectedSignal(NULL);
        }

    }
}
