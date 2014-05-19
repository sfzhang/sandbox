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
 * @file The interface of monitor window
 *
 * @author zhangshengfa
 * @date 2014-04-08
 */

#ifndef _MONITORWINDOW_H_
#define _MONITORWINDOW_H_

#include "DicomInstance.h"

#include <QWidget>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <QImage>
#include <cstdlib>

/**
 * @class SliceLocationCompare
 *
 * @brief This class provide comparison of slice location
 */
class SliceLocationComparator {

public:

    /**
     * @brief Override the () operator
     *
     * @param[in] key1 The first key
     * @param[in] key2 The second key
     *
     * @return The result of the comparison of key1 and keye2
     */
    bool operator()(const std::string& key1, const std::string& key2) const
    {
        double d1 = atof(key1.c_str());
        double d2 = atof(key2.c_str());

        return (d1 < d2);
    }
};

/** @typedef Dicom map: map<slice_location, pair<index, vector<DICOM instance> >, SliceLocationComparator > */
typedef std::map<std::string, std::pair<int, std::vector<DicomInstance *> >,  SliceLocationComparator> DcmMap;

/**
 * @class MonitorWindow
 *
 * @brief Monitor window is to display MR images when curing.
 */
class MonitorWindow: public QWidget {

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    MonitorWindow(QWidget *parent = NULL): QWidget(parent),  m_map_index(0), m_selected(-1), m_scale(0) {}

    /**
     * @brief destructor
     *
     * @param N/A
     *
     * @return N/A
     */
    ~MonitorWindow()
    {
        register unsigned int i = 0;
        for (DcmMap::iterator it = m_dcm_map.begin();
             it != m_dcm_map.end();
             it++) {
            for (i = 0; i < it->second.second.size(); i++) {
                free(it->second.second[i]);
            }
        }
    }

    /** @brief load dicom image */
    bool load(const std::string &dir);

    /** @brief load dicom image from buffer */
    bool load();

private:

    /** @brief Display some text */
    void displayText(QPainter &painter, const QRectF &rect, DicomInstance &dcm_inst);

signals:

    void imageSelectedSignal(DicomInstance *dcm_inst);

public slots:

    void setDicomImage();

protected:

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event );

    void wheelEvent(QWheelEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

private:

    DcmMap m_dcm_map;           /**< DICOM map */
    unsigned int m_map_index;   /**< Map index */
    int m_selected;             /**< Current select image */
    double m_scale;             /**< Image scale */
};

#endif /* !_MONITORWINDOW_H_ */
