#! /bin/sh
set -ex
DIRNAME=$(cd $(dirname $0);cd ..;pwd)/build/release
rm -rfv ${DIRNAME}
mkdir -p ${DIRNAME}
cd ${DIRNAME}
cmake -DCMAKE_BUILD_TYPE=Release -DPROF=ON ../..
make -j30 donutApp
./donutApp ../../_donut_sample/pi.donut
google-pprof --svg ./donutApp prof.out > prof.svg
xdg-open prof.svg
