@echo off
REM Builds the RescueNet app and the test runner (needs g++ on PATH, e.g. MSYS2 ucrt64).
cd /d "%~dp0"

set CORE=src\Location.cpp src\Road.cpp src\Route.cpp src\Graph.cpp src\MinHeap.cpp src\Dijkstra.cpp ^
 src\EvacuationRequest.cpp src\EvacuationQueue.cpp src\Shelter.cpp src\ShelterManager.cpp ^
 src\RoadReport.cpp src\ReportQueue.cpp src\ReportHashTable.cpp src\RescueNetSystem.cpp

g++ -std=c++17 -Wall -Wextra -Iinclude %CORE% src\main.cpp -o rescuenet.exe
if errorlevel 1 exit /b 1

g++ -std=c++17 -Wall -Wextra -Iinclude %CORE% tests\tests.cpp -o tests.exe
if errorlevel 1 exit /b 1

echo Built rescuenet.exe and tests.exe
