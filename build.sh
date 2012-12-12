#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)

BUILDDIR=$DIRNAME/build
if [ "$MSYSTEM" = "MINGW32" -o "$OS" = "Windows_NT" ] ; then
	CONFIGURE_CMD="python waf configure --out $BUILDDIR --boost-includes=/d/software/dev/applib/boost/include --boost-lib=/d/software/dev/applib/boost/lib"
else
	export set CC='ccache gcc'
	export set CXX='ccache g++'
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
python waf --out $BUILDDIR --progress ${VAR}_debug $* 2>&1
exit $?
