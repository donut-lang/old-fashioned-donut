#! /bin/sh
DIRNAME=$(cd $(dirname $0);cd ..;pwd)/build/release
export CPUPROFILE=/tmp/prof.out
RUN_PROG=${DIRNAME}/donut
${RUN_PROG} < ${DIRNAME}/../../_donut_sample/tarai.donut
pprof --dot ${RUN_PROG} /tmp/prof.out | dot -T png > ${DIRNAME}/prof.png
rm -f ${CPUPROFILE}
shotwell  ${DIRNAME}/prof.png
