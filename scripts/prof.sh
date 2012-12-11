#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)
RUN_PROG=${DIRNAME}/Linux/debug/chisa
${RUN_PROG}
pprof --dot ${RUN_PROG} ${DIRNAME}/prof.out > ${DIRNAME}/prof.dot
dot -T png ${DIRNAME}/prof.dot > ${DIRNAME}/prof.png
shotwell  ${DIRNAME}/prof.png
rm ${DIRNAME}/prof.dot ${DIRNAME}/prof.out
