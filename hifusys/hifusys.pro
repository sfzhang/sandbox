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
# qmake for hifusys
#

# include protobuf.pri if compile protobuf
# include(protobuf.pri)

# qmake template: lib or app
TEMPLATE = app

# CONFIG: debug, warn_on, no-qt, and suport mult-thread
CONFIG += debug \
		  warn_on

# need qt or not
# CONFIG =

# network
QT += network

# resource
RESOURCES = hifusys.qrc

# DESTDIR: where put targets, $(MAKE_HIFU_ROOT)/bin or $(MAKE_HIFU_ROOT)/lib
DESTDIR = $(MAKE_HIFU_ROOT)/bin

# preprocessor macros(-D)
DEFINES +=

# include path 
INCLUDEPATH += $(MAKE_HIFU_ROOT)/include \
               $(MAKE_HIFU_ROOT)/utils \
               $(MAKE_HIFU_ROOT)/cure

# depended library
LIBS += -L$(MAKE_HIFU_ROOT)/lib -llogger_mt -lconfig\
        -ldcmdata -ldcmimage -loflog -lofstd -ldcmjpeg -lijg8 -lijg12 \
        -lijg16 -ldcmimgle  -ltiff -lpng

# make clean
QMAKE_CLEAN += $(DESTDIR)$(TARGET)

# head files
HEADERS = MainWindow.h \
          PlanWindow.h \
          qdicomimage.h \
          DisplayWindow.h \
          DicomInstance.h \
          MonitorWindow.h \
          ThreadManager.h \
          TcpClient.h \
          HifuSysDefs.h

# source files
SOURCES = HifuSys.cpp \
          MainWindow.cpp \
          PlanWindow.cpp \
          qdicomimage.cpp \
          DisplayWindow.cpp \
          DicomInstance.cpp \
          MonitorWindow.cpp \
          ThreadManager.cpp \
          TcpClient.cpp

# TARGET
TARGET = hifusys
