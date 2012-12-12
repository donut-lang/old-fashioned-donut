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

MODE=$1
if [ "$MODE" = "debug" ] ; then
	shift
elif [ "$MODE" = "release" ] ; then
	shift
else
	MODE="debug"
fi

TARGET=$1
if [ -z $TARGET ] ; then
TARGET="build"
fi
shift

python waf --out $BUILDDIR --progress ${TARGET}_${MODE} $* 2>&1
exit $?
