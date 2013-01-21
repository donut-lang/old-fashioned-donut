#! /bin/sh
DIRNAME=$(cd $(dirname $0);cd ..;pwd)
PRG="${DIRNAME}/build/debug/chisa --trace slide"
GEN=--gen-suppressions=all

#valgrind -v --track-origins=yes --error-limit=no --leak-check=yes\
#--show-reachable=no ${PRG} 2>&1 | gedit
echo valgrind -v --error-limit=no --leak-check=full --show-reachable=yes \
--suppressions=${DIRNAME}/scripts/_valgrind_sup.txt ${GEN} -- ${PRG} 2>&1

valgrind -v --error-limit=no --leak-check=full --show-reachable=yes \
--suppressions=${DIRNAME}/scripts/_valgrind_sup.txt ${GEN} -- ${PRG} 2>&1 | gedit

