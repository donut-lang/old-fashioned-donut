#! /bin/sh
DIRNAME=$(cd $(dirname $0);cd ..;pwd)

function filter() {
	Pattern=$1
	lcov -r /tmp/cov.info "${Pattern}" -o /tmp/cov_tmp
	mv /tmp/cov_tmp /tmp/cov.info
}

echo now: `pwd`
lcov -c -f -b ${DIRNAME}/build/debug -d ${DIRNAME}/build/debug -o /tmp/cov.info
filter "*/usr/*"
filter "*/external/*"
rm cov/ -rf
genhtml -o cov -f /tmp/cov.info
rm /tmp/cov.info
find . -type f -name *.gcov | xargs rm

