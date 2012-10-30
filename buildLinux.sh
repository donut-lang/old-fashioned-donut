#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)
BUILDDIR=$DIRNAME/Linux
if [ ! -e $BUILDDIR ]; then
	echo "not configured"
	python waf configure --out $BUILDDIR
else
	echo "already configured"
fi
VAR=$1
if [ -z $VAR ] ; then
VAR="build"
fi
shift
python waf --out $BUILDDIR -p ${VAR}_debug $*

