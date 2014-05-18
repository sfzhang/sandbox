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
# Git smudge filter
#

# ident expand
log=`git log -1 --pretty=format:"%h %ad %an" --date=iso -- $1 | sed 's/ +[0-9]\{4\}//g'`
info='$Id$1`" $log"'$'

cat /dev/stdin | sed "s/\\\$Id\\\$/$info/g"
