@echo off
echo Compiling the program...
g++ main.cpp bin/lib/func/Administrator.cpp bin/lib/func/Candidate.cpp bin/lib/func/Election.cpp bin/lib/func/ElectionManager.cpp bin/lib/func/LocalElection.cpp bin/lib/func/NationalElection.cpp bin/lib/func/RegionalElection.cpp bin/lib/func/User.cpp bin/lib/func/Voter.cpp bin/lib/func/VotingSystem.cpp bin/lib/func/timeUtils.cpp -o OnlineVotingSystem.exe -I bin/lib/header

if %errorlevel% equ 0 (
    echo Build successful! Run the program using: .\OnlineVotingSystem.exe
) else (
    echo Build failed!
)
