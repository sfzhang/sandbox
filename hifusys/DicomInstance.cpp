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
 * @file The implement of DICOM instance
 *
 * @author zhangshengfa
 * @date 2014-04-08
 */

#include <dcmtk/config/cfunix.h>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/ofstd/ofconsol.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmdata/dcrledrg.h>

#include "DicomInstance.h"
#include "HifuSysDefs.h"
#include <logger/Logger.h>

#include <QImage>

void DicomInstance::destroy()
{
    if (m_dcm_file_format) {
        delete m_dcm_file_format;
        m_dcm_file_format = NULL;
    }

    if (m_dcm_image) {
        delete m_dcm_image;
        m_dcm_image = NULL;
    }
}

void DicomInstance::render()
{
    if (!m_dcm_image) {
        return;
    }

    register unsigned long i = 0;
    register unsigned long j = 0;

    uchar* pixel_data = (uchar *)(m_dcm_image->getOutputData(8));
    uchar* bits = m_qimage.bits();
    if (m_dcm_image->isMonochrome()) {
        for (i = 0; i < m_dcm_image->getHeight(); i++) {
            for(j = m_dcm_image->getWidth(); j != 0; j--) {
                *bits++ = *pixel_data;  /* R */
                *bits++ = *pixel_data;  /* G */
                *bits++ = *pixel_data;  /* B */
                *bits++ = 0xFF;         /* Alpha */

                pixel_data++;
            }
        }
    }
    else {
        for(i = m_qimage.width() * m_qimage.height(); i != 0; i--) {
            *bits++ = *pixel_data++ ;   /* R */
            *bits++ = *pixel_data++;    /* G */
            *bits++ = *pixel_data++;    /* B */
            *bits++ = 0xFF;             /* Alpha */
        }
    }
}

DicomInstance::DicomInstance():
    m_dcm_file_format(NULL),
    m_dcm_image(NULL)
{
}

DicomInstance::~DicomInstance()
{
    destroy();
}

bool DicomInstance::load(const QString& file)
{
    /* destroy() first */
    destroy();
    m_file = file;

    /* allocate new resource */
    m_dcm_file_format = new DcmFileFormat();
    OFCondition cond = m_dcm_file_format->loadFile(file.toAscii(),
                                                   EXS_Unknown,
                                                   EGL_withoutGL,
                                                   DCM_MaxReadLength);

    if (!cond.good()) {
        ELOG("load() failed: file[%s], reason[%s]", file.toLatin1().data(), cond.text());
        destroy();

        return false;
    }
        
    m_dcm_image = new DicomImage(m_dcm_file_format, m_dcm_file_format->getDataset()->getOriginalXfer());
    EI_Status status = m_dcm_image->getStatus();
    if (status != EIS_Normal) {
        ELOG("new DicomImage() failed: file[%s], status[%s]", file.toLatin1().data(),
             m_dcm_image->getString(status));
        destroy();
        
        return false;
    }

    m_qimage = QImage(m_dcm_image->getWidth(), m_dcm_image->getHeight(),
                      QImage::Format_ARGB32);

    DLOG("load() success: file[%s]", file.toLatin1().data());
    return true;
}

bool DicomInstance::load(const QString &file, const void *buffer, unsigned int size)
{
    if (!file.length() || !buffer || !buffer) {
        ELOG("Argument(s) illegal: file[%s], buffer[%lp], size[%u]",
             TO_PCHAR(file), buffer, size);
		return false;
	}

    /* destroy() first */
    destroy();

    /* allocate new resource */
    m_dcm_file_format = new DcmFileFormat();

	DcmInputBufferStream dcm_stream;
	dcm_stream.setBuffer(buffer, size);
	dcm_stream.setEos();

	m_dcm_file_format->transferInit();
	OFCondition condition = m_dcm_file_format->read(dcm_stream);
	m_dcm_file_format->transferEnd();

	if (!condition.good()) {
        ELOG("read() failed: file[%s], condition[%s]", TO_PCHAR(file), condition.text());
        destroy();
		return false;
	}

	m_dcm_image = new DicomImage(m_dcm_file_format->getDataset(),
								   m_dcm_file_format->getDataset()->getOriginalXfer());
    EI_Status status = m_dcm_image->getStatus();
    if (status != EIS_Normal) {
        ELOG("new DicomImage() failed: file[%s], status[%s]", file.toLatin1().data(),
             m_dcm_image->getString(status));
        destroy();
        
        return false;
    }

	m_file = file;
    m_qimage = QImage(m_dcm_image->getWidth(), m_dcm_image->getHeight(),
                      QImage::Format_ARGB32);

    DLOG("load() success: file[%s], buffer[%lp], size[%u]", TO_PCHAR(file), buffer, size);
    return true;
}

bool DicomInstance::getImageSize(unsigned long &width, unsigned long &height) const
{
    if (!m_dcm_image) {
        ELOG("m_dcm_image[%p] is NULL, load() DICOM image first!", m_dcm_image);
        return false;
    }

    width = m_dcm_image->getWidth();
    height = m_dcm_image->getHeight();
    DLOG("image width[%u], image height[%u]", width, height);

    return true;
}

bool DicomInstance::getWindow(double &center, double &width) const
{
    if (!m_dcm_image) {
        ELOG("m_dcm_image[%p] is NULL, load() DICOM image first!", m_dcm_image);
        return false;
    }

    int ret = m_dcm_image->getWindow(center, width);
    if (!ret) {
        ELOG("getWindow() failed: ret[%d]", ret);
        return false;
    }
    DLOG("getWindow() success: window center[%f], window width[%f]", center, width);
    return true;
}

bool DicomInstance::setWindow(double center, double width)
{
    if (!m_dcm_image) {
        ELOG("m_dcm_image[%p] is NULL, load() DICOM image first!", m_dcm_image);
        return false;
    }

    int ret = m_dcm_image->setWindow(center, width);
    if (!ret) {
        ELOG("setWindow() failed: ret[%d]", ret);
        return false;
    }
    DLOG("setWindow() success: window center[%f], window width[%f]", center, width);

    /* Window changed */
    if (1 == ret) {
        render();
    }

    return true;
}

bool DicomInstance::setMinMaxWindow()
{
    if (!m_dcm_image) {
        ELOG("m_dcm_image[%p] is NULL, load() DICOM image first!", m_dcm_image);
        return false;
    }

    int ret = m_dcm_image->setMinMaxWindow();
    if (!ret) {
        ELOG("setMinMaxWindow() failed: ret[%d]", ret);
        return false;
    }
    DLOG("setMinMaxWindow() success");

    /* Window changed */
    if (1 == ret) {
        render();
    }

    return true;
}

const QImage& DicomInstance::getOutputData() const
{
    return m_qimage;
}

bool DicomInstance::getTagValue(const DcmTagKey &tag, std::string &value) const
{
	OFString dcm_tag_value;
	OFCondition condition = m_dcm_file_format->getDataset()->findAndGetOFStringArray(tag, dcm_tag_value);
	if (condition.good()) {
		value = dcm_tag_value.c_str();
		return true;
	}
	else {
		return false;
	}
}

bool DicomInstance::getTagValue(const DcmTagKey &tag, QString &value) const
{
	OFString dcm_tag_value;
	OFCondition condition = m_dcm_file_format->getDataset()->findAndGetOFStringArray(tag, dcm_tag_value);
	if (condition.good()) {
		value = dcm_tag_value.c_str();
		return true;
	}
	else {
		return false;
	}
}
