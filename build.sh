#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)

if [ "$MSYSTEM" = "MINGW32" -o "$OS" = "Windows_NT" ] ; then
	BUILDDIR=$DIRNAME/Win
	CONFIGURE_CMD="python waf configure --out $BUILDDIR --boost-includes=/d/software/dev/applib/boost/include --boost-lib=/d/software/dev/applib/boost/lib"
else
	BUILDDIR=$DIRNAME/Linux
	CONFIGURE_CMD="python waf configure --out $BUILDDIR"
fi

if [ ! -e $BUILDDIR ]; then
	echo "not configured"
	$CONFIGURE_CMD
else
	echo "already configured"
fi

VAR=$1
if [ -z $VAR ] ; then
VAR="build"
fi
shift
python waf --out $BUILDDIR -p ${VAR}_debug $*

