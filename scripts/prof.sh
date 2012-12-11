#! /bin/sh
DIRNAME=$(cd $(dirname $0);cd ..;pwd)
RUN_PROG=${DIRNAME}/build/debug/chisa
${RUN_PROG}
pprof --dot ${RUN_PROG} ${DIRNAME}/prof.out > /tmp/prof.dot
rm ${DIRNAME}/prof.out
dot -T png /tmp/prof.dot > ${DIRNAME}/prof.png
rm /tmp/prof.dot
shotwell  ${DIRNAME}/prof.png
