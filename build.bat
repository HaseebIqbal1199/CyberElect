@echo off
echo Compiling the program...
g++ main.cpp bin/lib/func/*.cpp -o OnlineVotingSystem.exe -I.

if %errorlevel% equ 0 (
    echo Build successful! Run the program using: .\OnlineVotingSystem.exe
) else (
    echo Build failed!
)
