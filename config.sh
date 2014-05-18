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
# Config hifu
#

# Project config
# project path
echo "Please input your project path: `pwd`"
read project_path
if [ "$project_path" = "" ]; then
	project_path=`pwd`
fi
echo "Your project path: $project_path"

echo "export PATH=\$PATH:$project_path/sbin" >> $HOME/.bashrc
export PATH=$PATH:$project_path/sbin

cp $project_path/sbin/git_indent_clean.sh $project_path/sbin/.git_indent_clean.sh
cp $project_path/sbin/git_indent_smudge.sh $project_path/sbin/.git_indent_smudge.sh

# common config
# user name
echo "Please input your git user name:"
read user_name
git config --global user.name $user_name
if [ $? -ne 0 ]; then
	echo "git config --global user.name failed: $?"
	exit 1
fi
echo "Your git user name is:"
git config --global user.name

# user email
echo "Please input your git user email:"
read user_email
git config --global user.email $user_email
if [ $? -ne 0 ]; then
	echo "git config --global user.email failed: $?"
	exit 1
fi
echo "Your git user email is:"
git config --global user.email

# editor
echo "Please input your favorite(or familiar) editor: vim"
read editor
if [ "$editor" = "" ]; then
   editor=vim
fi   
git config --global core.editor $editor
if [ $? -ne 0 ]; then
	echo "git config --global core.editor failed: $?"
	exit 1
fi
echo "Your git editor is:"
git config --global core.editor

# merge tool
echo "Please input your favorite(or familiar) merge tool: vimdiff"
read merge_tool
if [ "$merge_tool" = "" ]; then
	merge_tool=vimdiff
fi
git config --global merge.tool $merge_tool
if [ $? -ne 0 ]; then
	echo "git config --global merge.tool failed: $?"
	exit 1
fi
echo "Your git merge tool is:"
git config --global merge.tool

# color
git config --global color.ui true
if [ $? -ne 0 ]; then
	echo "git config --global color.ui failed: $?"
	exit 1
fi

# crlf
git config --global core.autocrlf input
if [ $? -ne 0 ]; then
	echo "git config --global core.autocrlf failed: $?"
	exit 1
fi

git config --global core.safecrlf true
if [ $? -ne 0 ]; then
	echo "git config --global core.safecrlf failed: $?"
	exit 1
fi

# eol
git config --global core.eol lf
if [ $? -ne 0 ]; then
	echo "git config --global core.eol failed: $?"
	exit 1
fi

# filter
git config filter.indent.clean '.git_indent_clean.sh %f'
if [ $? -ne 0 ]; then
	echo "git config filter.indent.clean failed: $?"
	exit 1
fi

git config filter.indent.smudge '.git_indent_smudge.sh %f'
if [ $? -ne 0 ]; then
	echo "git config filter.indent.smudge failed: $?"
	exit 1
fi
