@echo off
setlocal

if NOT EXIST "build" (
    mkdir build
)

cmake -S . -B build -G "Ninja"
cmake --build build

endlocal
