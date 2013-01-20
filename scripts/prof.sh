#! /bin/sh
DIRNAME=$(cd $(dirname $0);cd ..;pwd)/build/release
export CPUPROFILE=/tmp/prof.out
RUN_PROG=${DIRNAME}/chisa
${RUN_PROG}
pprof --dot ${RUN_PROG} /tmp/prof.out | dot -T png > ${DIRNAME}/prof.png
rm -f ${CPUPROFILE}
shotwell  ${DIRNAME}/prof.png

