#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)

function conf() {
	BUILDDIR=$DIRNAME/build
	if [ "$MSYSTEM" = "MINGW32" -o "$OS" = "Windows_NT" ] ; then
		CONFIGURE_CMD="python waf configure --out $BUILDDIR --boost-includes=/d/software/dev/applib/boost/include --boost-lib=/d/software/dev/applib/boost/lib"
	else
		export set CC='ccache gcc'
		export set CXX='ccache g++'
		CONFIGURE_CMD="python waf configure --out $BUILDDIR"
	fi
	echo $CONFIGURE_CMD
	$CONFIGURE_CMD
}

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

BLD_WAF_CMD="python waf --out $BUILDDIR --progress ${TARGET}_${MODE} $*"
echo $BLD_WAF_CMD
$BLD_WAF_CMD 2>&1
if [ $? -ne 0 ] ; then
	echo "not configured"
	conf
	echo $BLD_WAF_CMD
	$BLD_WAF_CMD 2>&1
fi
exit $?

