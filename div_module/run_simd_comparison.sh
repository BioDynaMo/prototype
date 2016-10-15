#!/bin/bash

echo "SSE"
make clean >/dev/null ; rm CMakeCache.txt >/dev/null ; \
 cmake -Dtest=off -DCMAKE_CXX_FLAGS="-O3" -DCMAKE_BUILD_TYPE=Debug .. >/dev/null 2>&1 && \
 make -j5 >/dev/null && \
 sleep 1 && ./divModule --full-analysis

echo "AVX"
make clean >/dev/null ; rm CMakeCache.txt >/dev/null ; \
 cmake -Dtest=off -DCMAKE_CXX_FLAGS="-O3 -mavx2" -DCMAKE_BUILD_TYPE=Debug .. >/dev/null 2>&1 && \
 make -j5 >/dev/null && \
 sleep 1 && ./divModule --full-analysis
