#! /bin/sh
DIRNAME=$(cd $(dirname $0);pwd)
PRG=${DIRNAME}/Linux/debug/chisa_test
GEN=--gen-suppressions=all

#valgrind -v --track-origins=yes --error-limit=no --leak-check=yes --show-reachable=no ${PRG} 2>&1 | tee ${DIRNAME}/valgrind.log
valgrind -v --error-limit=no --leak-check=full --show-reachable=yes --suppressions=_valgrind_sup.txt ${GEN} ${PRG} 2>&1 | tee ${DIRNAME}/valgrind.log

