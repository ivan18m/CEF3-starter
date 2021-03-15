@echo off
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019"  ../.. -B ../../build
pause