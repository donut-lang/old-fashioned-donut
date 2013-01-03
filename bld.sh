#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)

BUILDDIR=$DIRNAME/build

CLANG_FOUND=
if which clang++ 2>&1 > /dev/null ; then
	echo "clang found."
	#CLANG_FOUND=1
else
	echo "clang not found."
fi

CCACHE_FOUND=
if which ccache 2>&1 > /dev/null ; then
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

function conf() {
	if [ "$MSYSTEM" = "MINGW32" -o "$OS" = "Windows_NT" ] ; then
		CONFIGURE_CMD="python waf configure --out $BUILDDIR --boost-includes=/d/software/dev/applib/boost/include --boost-lib=/d/software/dev/applib/boost/lib"
	else
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
else
	shift
fi

BLD_WAF_CMD="python waf --out $BUILDDIR --progress ${TARGET}_${MODE} $*"
if [ ! -d $BUILDDIR ] ; then
	echo "not configured"
	conf
fi
echo $BLD_WAF_CMD
$BLD_WAF_CMD 2>&1
exit $?

