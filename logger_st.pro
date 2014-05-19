#
#  Copyright (c) 2013, BLK Co., Ltd.  All right reserved.
#
#  This is unpublished proprietary information of BLK CO., Ltd and is
#  protected as an unpublished work under applicable Copyright laws.
#  The contents of this file may not be disclosed to third parties,
#  copied or duplicated in any form, in whole or in part, without the
#  prior written permission of BLK Co., Ltd.
#
#  $Id$
#

#
# qmake for project logger
#

# qmake template
TEMPLATE = lib

# CONFIG: debug, warn_on, no-qt
CONFIG += debug \
		  warn_on \
		  plugin
CONFIG -= qt

# DESTDIR: where put targets
DESTDIR = $(MAKE_HIFU_ROOT)/lib

# preprocessor macros(-D)
DEFINES += LOGGER_EXPORT

# include path 
INCLUDEPATH += $(MAKE_HIFU_ROOT)/include

# depended library
LIBS +=

# object directory
OBJECTS_DIR = ./logger_st_obj

# make clean
QMAKE_CLEAN += $(DESTDIR)$(TARGET)

# head files
HEADERS = LoggerDefs.h \
          Logger.h \
          FileAppender.h \
          Trace.h

# source files
SOURCES = ModDll.cpp \
          Logger.cpp \
          FileAppender.cpp \
          Format.inc

# TARGET
TARGET = logger
