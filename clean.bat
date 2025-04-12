@echo off
echo Cleaning up the project...

rem Try to kill any running instances of the program
taskkill /F /IM OnlineVotingSystem.exe >nul 2>&1

rem Wait a moment for the process to fully terminate
timeout /t 1 /nobreak >nul

rem Delete the executable if it exists
if exist OnlineVotingSystem.exe (
    echo Removing executable...
    del /F OnlineVotingSystem.exe 2>nul
    if exist OnlineVotingSystem.exe (
        echo Warning: Could not remove executable. It may be in use.
    ) else (
        echo Executable removed successfully.
    )
)

rem Delete data directory if it exists
if exist data (
    echo Removing data directory...
    rmdir /s /q data 2>nul
)

echo Cleanup complete!
