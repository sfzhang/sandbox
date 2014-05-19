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
 * @file Some definition for project logger
 *
 * @author zhangshengfa
 * @date 2013-12-10
 */

#ifndef _LOGGERDEFS_H_
#define _LOGGERDEFS_H_

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif

#include <HifuDefs.h>

/** @brief Macro for dll export and import */
#ifdef  LOGGER_EXPORT
#	define LOGGER_LIB		HIFU_LIB_EXPORT
#else
#	define LOGGER_LIB		HIFU_LIB_IMPORT
#endif /* !LOGGER_EXPORT */

/** @brief Macro maximum queue size */
#define MAX_QUEUE_SIZE		(1024 * 1024)
#define MAX_FILES_SIZE		(2 * 1024 * 1024 * 1024UL)
#define DFT_ROTATE_SIZE		(20 * 1024 * 1024)
#define MAX_LOG_SIZE        (50 * 1024 * 1024)

/** @brief Macro to dump the current function name */
#ifdef _MSC_VER
#   define __FN__   __FUNCTION__
#else
#   define __FN__   __func__
#endif /* !_MSC_VER */

#endif /* !_LOGGERDEFS_H_ */
