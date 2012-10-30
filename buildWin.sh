#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)
BUILDDIR=$DIRNAME/Win
if [ ! -e $BUILDDIR ]; then
	echo "not configured"
	python waf configure --out $BUILDDIR --boost-includes=/d/software/dev/applib/boost/include --boost-lib=/d/software/dev/applib/boost/lib
else
	echo "already configured"
fi
python waf --out $BUILDDIR $*
