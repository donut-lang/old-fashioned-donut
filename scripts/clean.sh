#!/usr/bin/env bash
set -x

# OSX
find . -name .DS_Store -delete

# Cmake
rm -Rf CMakeFiles CMakeScripts *.cmake CMakeCache.txt

# Xcode
rm -Rf Debug Donut.build Donut.xcodeproj

# Ninja
rm -Rf build.ninja .ninja_deps .ninja_log rules.ninja

# Makefile
rm -f Makefile

# binaries
rm -f donutApp testApp *.a
