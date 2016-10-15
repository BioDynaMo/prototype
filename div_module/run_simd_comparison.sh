#!/bin/bash

echo "SSE"
make clean >/dev/null ; rm CMakeCache.txt >/dev/null ; \
 cmake -Dtest=off -DCMAKE_CXX_FLAGS="-O3" -DCMAKE_BUILD_TYPE=Debug .. >/dev/null 2>&1 && \
 make -j5 >/dev/null && \
 sleep 1 && ./divModule --scaling-analysis

# /opt/intel/vtune_amplifier_xe_2016.1.1.434111/bin64/amplxe-cl -collect advanced-hotspots -knob collection-detail=stack-sampling -app-working-dir /home/lukas/Documents/cern/project/workspace/bdm_prototype/div_module/build_gcc -- /home/lukas/Documents/cern/project/workspace/bdm_prototype/div_module/build_gcc/divModule

echo "AVX"
make clean >/dev/null ; rm CMakeCache.txt >/dev/null ; \
 cmake -Dtest=off -DCMAKE_CXX_FLAGS="-O3 -mavx2" -DCMAKE_BUILD_TYPE=Debug .. >/dev/null 2>&1 && \
 make -j5 >/dev/null && \
 sleep 1 && ./divModule --scaling-analysis

# /opt/intel/vtune_amplifier_xe_2016.1.1.434111/bin64/amplxe-cl -collect advanced-hotspots -knob collection-detail=stack-sampling -app-working-dir /home/lukas/Documents/cern/project/workspace/bdm_prototype/div_module/build_gcc -- /home/lukas/Documents/cern/project/workspace/bdm_prototype/div_module/build_gcc/divModule
