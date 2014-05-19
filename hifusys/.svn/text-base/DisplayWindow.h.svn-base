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
 * @file The interface of display window
 *
 * @author zhangshengfa
 * @date 2014-02-24
 */

#ifndef _DISPLAYWINDOW_H_
#define _DISPLAYWINDOW_H_

#include <QWidget>
#include <vector>
#include <string>
#include <QImage>
#include "qdicomimage.h"

class DisplayWindow: public QWidget {

    Q_OBJECT

public:

    /**
     * @brief Default constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    DisplayWindow(QWidget *parent = NULL): QWidget(parent), m_selected_image_index(-1) {}

    /** @brief destructor */
    ~DisplayWindow()
    {
        for (size_t i = 0; i < m_dicom_image_vector.size(); i++) {
            free(m_dicom_image_vector[i]);
        }
    }

    /** @brief load dicom image */
    bool load(const std::string &dir);

    /** @brief sizeHint() */
    virtual QSize sizeHint() const;

signals:

    void imageSelectedSignal(QDicomImage *dicom_image);

protected:

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event );

private:

    std::vector<QDicomImage*> m_dicom_image_vector;
    std::vector<QImage> m_image_vector;

    int m_selected_image_index;

};

#endif /* !_DISPLAYWINDOW_H_ */
