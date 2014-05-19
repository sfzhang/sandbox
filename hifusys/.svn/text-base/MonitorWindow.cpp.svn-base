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
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <QtDebug>
#include <dcmtk/config/cfunix.h>
#include <dcmtk/dcmdata/dctk.h>
#include "MonitorWindow.h"

using namespace hifu::logger;

static void createImageLayout(unsigned int win_width, unsigned int win_height,
                               unsigned int image_width, unsigned int image_height,
                               unsigned int image_count, double &scale)
{
    /* Algorithm: find the maximum image scale k
     * row * column >= image_count
     * row = (win_width - 1) / (k * image_width + 1)
     * column = (win_height - 1) / (k * image_height + 1)
     */

    assert((win_width > 0) && (win_height > 0) &&
           (image_width > 0) && (image_height > 0) &&
           (image_count > 0));

    /* Width iterator */
    register unsigned int i = 0;
    register unsigned int j = 0;
    for (i = 1; i <= image_count; i++) {
        scale = 1.0 * (win_width - 1 - i) / i / image_width;
        j = 1.0 * (win_height - 1) / (scale * image_height + 1);

        if (0 == j) {
            scale = 0;
            continue;
        }

        if (i * j >= image_count) {
            break;
        }
    }
    DLOG("scale[%f]", scale);

    /* Heigth iterator */
    double s = 0;
    for (i = 1; i <= image_count; i++) {
        s = 1.0 * (win_height - 1 - i) / i / image_height;
        if (s <= scale) {
            break;
        }

        j = 1.0 * (win_width - 1) / (s * image_width + 1);

        if (0 == j) {
            s = 0;
            continue;
        }

        if (i * j >= image_count) {
            scale = s;
            break;
        }
    }

    DLOG("win: width[%u], height[%u]; img: width[%u], height[%u]; count[%u], scale[%f]",
         win_width, win_height, image_width, image_height, image_count, scale);
}

bool MonitorWindow::load(const std::string &dir)
{
    /* Slice loaction */
    std::string slice_location;

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
    int i = 0;
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
            continue;
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

        /* Get slice location */
        if (!dcm_inst->getTagValue(DCM_SliceLocation, slice_location)) {
            ELOG("getTagValue() failed: tag[DCM_SliceLocation]");
            return false;
        }

        /* Add to map */
        if (m_dcm_map.find(slice_location) == m_dcm_map.end()) {
            m_dcm_map[slice_location].first = 0;
        }
        m_dcm_map[slice_location].second.push_back(dcm_inst);

        dcm_inst->setMinMaxWindow();
        i++;
    }

    DLOG("count[%d]", i);
	closedir(dp);
	return true;
}

bool MonitorWindow::load()
{
    const char *file = "/home/sfzhang/SVN/hifu/dicom/dicom.20140323/s423369/i423373.MRDC.1";
    FILE *fp = fopen(file, "r");
    if (!fp) {
        ELOG("fopen() failed: file[%s], errno[%d]", file, errno);
        return false;
    }

    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *buffer = new char[size];

    fread(buffer, sizeof(char), size, fp);
    fclose(fp);


    DicomInstance *dcm_inst = new DicomInstance();
    if (!dcm_inst->load("i423373.MRDC.1", buffer, size)) {
        ELOG("load() failed");
        delete buffer;
        delete dcm_inst;
        return false;
    }

    /* Get slice location */
    std::string slice_location;
    if (!dcm_inst->getTagValue(DCM_SliceLocation, slice_location)) {
        ELOG("getTagValue() failed: tag[DCM_SliceLocation]");
        return false;
    }

    /* Add to map */
    if (m_dcm_map.find(slice_location) == m_dcm_map.end()) {
        m_dcm_map[slice_location].first = 0;
    }
    m_dcm_map[slice_location].second.push_back(dcm_inst);

    dcm_inst->setMinMaxWindow();

    DLOG("load() success");
    update();
    return true;
}

void MonitorWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int w = width();
    int h = height();

    m_scale = 0;
    if (m_dcm_map.size() > 0) {

        DcmMap::iterator begin = m_dcm_map.begin();
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        begin->second.second[0]->getImageSize(image_width, image_height);
        createImageLayout(w, h, image_width, image_height, m_dcm_map.size(), m_scale);
        int column = (w - 1) / (m_scale * image_width + 1);
        int row = (h - 1) / (m_scale * image_height + 1);
        DLOG("h[%d], m_scale[%f], image_height[%lu], row[%d]", h, m_scale, image_height, row);
        int dw = w / column;
        int dh = h / row;
        int r = 0;
        int c = 0;
        int x = 0;
        int y = 0;

        DcmMap::iterator it;
        QString text;
        for (int i = 0; i < m_dcm_map.size(); i++) {
            it = begin;
            std::advance(it, i);

            r = i / column;
            c = i % column;

            x = c * dw;
            y = r * dh;

            painter.drawImage(QRect(x + (dw - m_scale * image_width) / 2, y + (dh - m_scale * image_height) / 2,
                                    m_scale * image_width, m_scale * image_height),
                              it->second.second[it->second.first]->getOutputData());
            painter.setPen(QColor(0x90, 0xEE, 0x90));
            painter.drawRect(QRectF(x + (dw - m_scale * image_width) / 2, y + (dh - m_scale * image_height) / 2,
                                    m_scale * image_width, m_scale * image_height));

            /*
            painter.setPen(QColor(255, 215, 0));
            text = QString::number(it->second.first);
            painter.drawText(x + (dw - m_scale * image_width) / 2 + 10, y + (dh - m_scale * image_height) / 2 + 20, text);
            */

            // painter.setPen(QColor(0xFF, 0xFF, 0xF0));
            painter.setPen(QColor(255, 215, 0));
            displayText(painter, QRectF(x + (dw - m_scale * image_width) / 2 + 2, y + (dh - m_scale * image_height) / 2 + 2,
                                    m_scale * image_width - 4, m_scale * image_height - 4), *(it->second.second[it->second.first]));
        }

        if ((m_selected >= 0) && (m_selected < m_dcm_map.size())) {
            r = m_selected / column;
            c = m_selected % column;

            x = c * dw;
            y = r * dh;

            painter.setPen(QColor(0xFF, 0x45, 0x00));
            painter.drawRect(QRectF(x + (dw - m_scale * image_width) / 2, y + (dh - m_scale * image_height) / 2,
                                    m_scale * image_width, m_scale * image_height));

        }
    }
}

void MonitorWindow::mousePressEvent(QMouseEvent *event)
{
    if (0 == m_dcm_map.size()) {
        return;
    }

    if (Qt::LeftButton == event->button()) {
        DcmMap::iterator begin = m_dcm_map.begin();
        int w = width();
        int h = height();
        unsigned long image_width = 0;
        unsigned long image_height = 0;
        begin->second.second[0]->getImageSize(image_width, image_height);
        int column = (w - 1) / (m_scale * image_width + 1);
        int row = (h - 1) / (m_scale * image_height + 1);
        int dw = w / column;
        int dh = h / row;
    
        int r = 0;
        int c = 0;
        int x = 0;
        int y = 0;
    
        m_selected = -1;
        for (register unsigned int i = 0; i < m_dcm_map.size(); i++) {
            r = i / column;
            c = i % column;
    
            x = c * dw;
            y = r * dh;
    
            QRectF rect = QRectF(x + (dw - m_scale * image_width) / 2, y + (dh - m_scale * image_height) / 2,
                                 m_scale * image_width, m_scale * image_height);
            if (rect.contains(event->posF())) {
                m_selected = i;
                break;
            }
        }
    
        update();
    }
}

void MonitorWindow::setDicomImage()
{
    if (!load("../dicom")) {
        ELOG("load() failed");
        return;
    }

    update();
}

void MonitorWindow::wheelEvent(QWheelEvent *event)
{
    if (m_dcm_map.size() > 0) {
	    int degrees = event->delta() / 8;
	    int steps = degrees / 15;

        DcmMap::iterator it = m_dcm_map.begin();
        if ((m_selected >= 0) && (m_selected < m_dcm_map.size())) {
            std::advance(it, m_selected);

            it->second.first += steps;
            if (it->second.first < 0) {
                it->second.first = 0;
            }
            else if (it->second.first >= it->second.second.size()) {
                it->second.first = it->second.second.size() - 1;
            }

            update();
        }
        else {
            int index = it->second.first + steps;

            for ( ; it != m_dcm_map.end(); it++) {
                it->second.first = index;
                if (it->second.first < 0) {
                    it->second.first = 0;
                }
                else if (it->second.first >= it->second.second.size()) {
                    it->second.first = it->second.second.size() - 1;
                }
            }
            update();
        }
    }
    else {
        QWidget::wheelEvent(event);
    }
}

void MonitorWindow::displayText(QPainter &painter, const QRectF &rect, DicomInstance &dcm_inst)
{
    /* Patient information */
    QString value, text;
    if (dcm_inst.getTagValue(DCM_PatientID, value)) {
        text += "ID: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_PatientName, value)) {
        text += "Name: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_PatientSex, value)) {
        text += "Sex: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_PatientBirthDate, value)) {
        text += "BirthDate: " + value + "\n";
    }

    painter.drawText(rect, Qt::AlignTop | Qt::AlignLeft, text);

    /* Image information */
    text = "";
    if (dcm_inst.getTagValue(DCM_SeriesNumber, value)) {
        text += "Series Number: " + value + "\n";
    }
    
    if (dcm_inst.getTagValue(DCM_AcquisitionNumber, value)) {
        text += "Acquisition Number: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_InstanceNumber, value)) {
        text += "Instance Number: " + value + "\n";
    }

    QString rows, columns;
    if (dcm_inst.getTagValue(DCM_Rows, rows) && dcm_inst.getTagValue(DCM_Columns, columns)) {
        text += "R/C: " + rows + "/" + columns + "\n";
    }

    double window_center = 0;
    double window_width = 0;
    if (dcm_inst.getWindow(window_center, window_width)) {
        text += "W/C: " + QString::number(window_center) + "" +
                "/" + QString::number(window_width) + "\n";
    }

    painter.drawText(rect, Qt::AlignTop | Qt::AlignRight, text);

    /* Orientation information */
    text = "";
    if (dcm_inst.getTagValue(DCM_ImagePositionPatient, value)) {
        text += "Image Position: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_ImageOrientationPatient, value)) {
        text += "Image Orientation: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_PatientPosition, value)) {
        text += "Patient Postion: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_SliceLocation, value)) {
        text += "Slice Location: " + value;
    }

    painter.drawText(rect, Qt::AlignBottom | Qt::AlignLeft, text);

    /* T1 information */
    text = "";
    if (dcm_inst.getTagValue(DCM_RepetitionTime, value)) {
        text += "Repetition Time: " + value + "\n";
    }

    if (dcm_inst.getTagValue(DCM_FlipAngle, value)) {
        text += "Flip Angle: " + value;
    }

    painter.drawText(rect, Qt::AlignBottom | Qt::AlignRight, text);
}

void MonitorWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (0 == m_dcm_map.size()) {
        return;
    }

    DcmMap::iterator it = m_dcm_map.begin();
    int w = width();
    int h = height();
    unsigned long image_width = 0;
    unsigned long image_height = 0;
    it->second.second[0]->getImageSize(image_width, image_height);
    int column = (w - 1) / (m_scale * image_width + 1);
    int row = (h - 1) / (m_scale * image_height + 1);
    int dw = w / column;
    int dh = h / row;

    int r = 0;
    int c = 0;
    int x = 0;
    int y = 0;

    m_selected = -1;
    for (register unsigned int i = 0; i < m_dcm_map.size(); i++) {
        r = i / column;
        c = i % column;

        x = c * dw;
        y = r * dh;

        QRectF rect = QRectF(x + (dw - m_scale * image_width) / 2, y + (dh - m_scale * image_height) / 2,
                             m_scale * image_width, m_scale * image_height);
        if (rect.contains(event->posF())) {
            m_selected = i;
            break;
        }
    }

    if (m_selected != -1) {
        std::advance(it, m_selected);
        emit imageSelectedSignal(it->second.second[it->second.first]);
    }
}
