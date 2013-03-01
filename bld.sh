#! /bin/bash

# Chisa
# Copyright (C) 2012 psi
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

PYTHON=python

function detectCompiler() {
	DIRNAME=$(cd $(dirname $0);pwd)
	BUILDDIR=$DIRNAME/build

	CLANG_FOUND=
	which clang++ > /dev/null 2>&1
	if [ $? -eq 0 ] ; then
		echo "clang found."
		#CLANG_FOUND=1
	else
		echo "clang not found."
	fi

	CCACHE_FOUND=
	which ccache > /dev/null 2>&1
	if [ $? -eq 0 ] ; then
		echo "ccache found."
		CCACHE_FOUND=1
	else
		echo "cchache not found."
	fi

	if [ ! -z $CLANG_FOUND ];then
		export set CC='clang'
		export set CXX='clang++'
	elif [ ! -z $CCACHE_FOUND ] ;then
		export set CC='ccache gcc'
		export set CXX='ccache g++'
	fi
}

function conf() {
	if [ "$MSYSTEM" = "MINGW32" -o "$OS" = "Windows_NT" ] ; then
		CONFIGURE_CMD="$PYTHON waf configure --out $BUILDDIR"
	else
		CONFIGURE_CMD="$PYTHON waf configure --out $BUILDDIR"
	fi
	echo "configure: $CONFIGURE_CMD"
	$CONFIGURE_CMD
}

function ensureConfigure() {
	if [ ! -d $BUILDDIR ] ; then
		echo "not configured"
		conf
		return 0
	fi
	$PYTHON ./waf list > /dev/null 2>&1
	if [ $? -ne 0 ] ;then
		echo "not configured"
		conf
		return $?
	fi
	echo "configured"
	return 0
}

function parse() {
	consumed=1
	if [ "$1" = "debug" ] ; then
		MODE=$1; consumed=0
	elif [ "$1" = "release" ] ; then
		MODE=$1; consumed=0
	elif [ "$1" = "build" ] ; then
		TARGET=$1; consumed=0
	elif [ "$1" = "clean" ] ; then
		TARGET=$1; consumed=0
	elif [ "$1" = "install" ] ; then
		TARGET=$1; consumed=0
	elif [ "$1" = "uninstall" ] ; then
		TARGET=$1; consumed=0
	elif [ ! -z "$1" ]; then
		export CUSTOM=1
	fi

	if [ -z "$MODE" ]; then
		MODE="debug"
	fi
	if [ -z "$TARGET" ]; then
		TARGET="build"
	fi
	return $consumed
}

# parsing command line
CUSTOM=0
parse $1 && shift
parse $1 && shift

if [ $CUSTOM -eq 0 ]; then
	echo "Normal launch"
	detectCompiler
	ensureConfigure
	BLD_WAF_CMD="$PYTHON ./waf --out $BUILDDIR --progress ${TARGET}_${MODE} $*"
else
	echo "Custom launch"
	BLD_WAF_CMD="$PYTHON ./waf --out $BUILDDIR --progress $*"
fi

echo "building: $BLD_WAF_CMD"
$BLD_WAF_CMD 2>&1
exit $?
