#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)

ANTLR_SRC=${DIRNAME}/src/chisa/donut/parser
ANTLR_OUT=${ANTLR_SRC}/output

CLASSPATH=${DIRNAME}/external/antlr/antlr-3.4-complete.jar:$CLASSPATH \
java org.antlr.Tool -fo ${ANTLR_OUT} -make \
	${ANTLR_SRC}/Donut.g ${ANTLR_SRC}/Compiler.g

find ${ANTLR_OUT} -type f -name *.c | while read f; do
  mv $f ${ANTLR_OUT}/$(basename $f .c).cpp
done

