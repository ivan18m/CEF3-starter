@echo off
cmake --build ../../build --config Release --target ALL_BUILD -- /maxcpucount:6
pause