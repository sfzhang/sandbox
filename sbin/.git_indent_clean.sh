#!/bin/bash

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
# Git clean filter
#

# indent clean
cat /dev/stdin | sed 's/\$Id[^\$]*\$/\$Id\$/g'
