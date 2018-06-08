#!/usr/bin/env bash
set -x

# OSX
find . -name .DS_Store -delete

# Xcode
rm -Rf Debug Donut.build Donut.xcodeproj CMakeFiles CMakeScripts *.cmake CMakeCache.txt
