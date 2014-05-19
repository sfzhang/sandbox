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

#ifndef _DICOMINSTANCE_H_
#define _DICOMINSTANCE_H_

/**
 * @file The interface of DICOM instance
 *
 * @author zhangshengfa
 * @date 2014-04-08
 */

#include <dcmtk/config/cfunix.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <QtGui>

class QImage;
class DcmFileFormat;
class DicomImage;

/**
 * @class DicomInstance
 *
 * @brief The DICOM instance to display
 */
class DicomInstance {

public:

    /**
     * @brief Constructor
     *
     * @param N/A
     *
     * @return N/A
     */
    DicomInstance();

    /**
     * @brief Destructor
     *
     * @param N/A
     *
     * @return N/A
     */
    ~DicomInstance();

    /**
     * @brief Load DICOM image
     *
     * @param[in] file The DICOM file name
     *
     * @retval true Load successfully
     * @retval false Failed to load
     */
    bool load(const QString& file);

    /**
     * @brief Override function, load DICOM from buffer
     *
     * @param[in] file The DICOM file name
     * @param[in] buffer The DICOM buffer
     * @param[in] size The size of buffer
     *
     * @retval true Load successfully
     * @retval false Failed to load
     */
    bool load(const QString &file, const void *buffer, unsigned int size);

    /**
     * @brief Get the DICOM file name
     *
     * @param N/A
     *
     * @return The DICOM file name
     */
    const QString& getFileName()
    {
        return m_file;
    }

    /**
     * @brief Get image width and image height
     *
     * @param[out] width The buffer saved image width
     * @param[out] height The buffer saved image height 
     *
     * @retval true Get image size successfully
     * @retval false Failed to get image size
     */
    bool getImageSize(unsigned long &width, unsigned long &height) const;

    /**
     * @brief Get window center and window width
     *
     * @param[out] center The buffer saved window center
     * @param[out] width The buffer saved window width
     *
     * @retval true Get window center and window width successfully
     * @retval false Failed to get window center and window width
     */
    bool getWindow(double &center, double &width) const;
    
    /**
     * @brief Set window center and window width
     *
     * @param[in] center The window center
     * @param[in] width The window width
     *
     * @retval true Set window center and window width successfully
     * @retval false Failed to set window center and window width
     */
    bool setWindow(double center, double width);
    
    /**
     * @brief Set window center and window width use min-max algorithm
     *
     * @param N/A
     *
     * @retval true Set window center and window width successfully
     * @retval false Failed to set window center and window width
     */
    bool setMinMaxWindow();

    /**
     * @brief Get Tag value
     *
     * @param tag The Tag key
     * @param value The value of specified tag
     *
     * @retval true Get the tag value successfully
     * @retval false Failed to get the tag
     */
    bool getTagValue(const DcmTagKey &tag, std::string &value) const;

    /**
     * @brief Overload get Tag value
     *
     * @param tag The Tag key
     * @param value The value of specified tag with type QString
     *
     * @retval true Get the tag value successfully
     * @retval false Failed to get the tag
     */
    bool getTagValue(const DcmTagKey &tag, QString &value) const;

    /**
     * @brief Get the image after render
     *
     * @param N/A
     *
     * @retval The render QImage
     */
    const QImage& getOutputData() const;

protected:

    /**
     * @brief Destroy resource
     *
     * @param N/A
     *
     * @return N/A
     */
    void destroy();

    /**
     * @brief Render the image
     *
     * @param N/A
     *
     * @return N/A
     */
    void render();

protected:

    QString m_file;                     /**< DICOM file name */
    QImage m_qimage;                    /**< QImage for DICOM display */
    DcmFileFormat *m_dcm_file_format;   /**< DICOM file format */
    DicomImage *m_dcm_image;            /**< DICOM image */ 

};

#endif /*! _DICOMINSTANCE_H_ */
