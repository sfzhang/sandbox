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
 * @file Dynamic link library entry for Windows
 *
 * @author zhangshengfa
 * @date 2013-12-12
 */

#include "LoggerDefs.h"

#ifdef WIN32

#include <Windows.h>

/**
 * @brief DLL entry point for Windows
 *
 * @param[in] hInstDll   DLL module handle
 * @param[in] dwReason   The reason code for call the DLL entry point
 * @param[in] lpReserved Address for dwReason
 *
 * @return If the process load library successfully, the dwReason is
 * 		   DLL_PROCESS_ATTACH, return TRUE, if the process failed to
 * 		   load library, it return FALSE, other reason, the return
 * 		   value is ignored
 */
BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default:
			break;
	}

	return TRUE;
}

#endif /* !WIN32 */
