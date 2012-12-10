#! /bin/sh
#! /bin/sh

function filter() {
	Pattern=$1
	lcov -r cov.info "${Pattern}" -o cov_tmp
	mv cov_tmp cov.info
}

echo now: `pwd`
lcov -c -f -b build/debug -d build/debug -o cov.info
filter "*/usr/*"
filter "*/external/*"
rm cov/ -rf
genhtml -o cov -f cov.info
rm cov.info
find . -type f -name *.gcov | xargs rm

