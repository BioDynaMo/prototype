```
export PATH=/afs/cern.ch/sw/lcg/app/releases/GEANTV-externals/externals-testjuin2016/Vc/1.2.0/x86_64-slc6-gcc49-opt/lib/cmake/Vc/:$PATH
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/gcc/4.8/x86_64-slc6-gcc48-opt/lib64:$LD_LIBRARY_PATH
# remove ROOT from CMakeLists.txt
cmake -DCMAKE_CXX_COMPILER=/afs/cern.ch/sw/lcg/external/gcc/4.8/x86_64-slc6-gcc48-opt/bin/g++ -DCMAKE_C_COMPILER=/afs/cern.ch/sw/lcg/external/gcc/4.8/x86_64-slc6-gcc48-opt/bin/gcc ..
make
```

clang
```
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/gcc/5.2/x86_64-slc6/lib64/
cmake -DCMAKE_C_COMPILER=/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-slc6/bin/clang -DCMAKE_CXX_COMPILER=/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-slc6/bin/clang++ ..
```

icc
https://twiki.cern.ch/twiki/bin/view/Sandbox/GeorgiosBItzesSandbox
```
source /afs/cern.ch/sw/IntelSoftware/linux/all-setup.sh
cmake -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc ..
```