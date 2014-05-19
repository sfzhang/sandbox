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
# qmake for project utils
#

# qmake template
TEMPLATE = subdirs

# CONFIG: orderd
CONFIG +=

# SUBDIRS
SUBDIRS = logger_st \
          logger_mt
logger_st.file = logger_st.pro
logger_mt.file = logger_mt.pro
