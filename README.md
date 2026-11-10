# RescueNet

C++17 console simulation for disaster evacuation: a city road graph, Dijkstra
routing over open roads, a priority queue of evacuation requests, shelter
allocation, and a verified road-report workflow that reroutes groups when roads
are blocked.

## Build and run (Windows, MSYS2 g++)

Needs `g++` on PATH (e.g. `C:\msys64\ucrt64\bin`). Run from the project root.

```
build.bat
rescuenet.exe            # uses data/city.txt
rescuenet.exe my_map.txt # or any map file
tests.exe                # run from this folder
```

## Project layout

```
include/   class headers (.h)
src/       class sources (.cpp) and main.cpp
tests/     tests.cpp (self-checking test runner)
data/      city.txt (the sample city map)
build.bat  builds rescuenet.exe and tests.exe
```

## Modules by milestone

| Milestone | Files | Data structure |
|---|---|---|
| M1 domain classes | Location, Road, Route, EvacuationRequest, Shelter, RoadReport | — |
| M2 graph | Graph | custom adjacency list (linked lists) |
| M3 routing | MinHeap, Dijkstra | binary min-heap |
| M4 evacuation | EvacuationQueue, ShelterManager | binary max-heap priority queue |
| M5 road reports | ReportQueue, ReportHashTable, RescueNetSystem | linked-list FIFO queue, chained hash table |
| M6 integration | main.cpp, tests/tests.cpp, build.bat | — |

## Map file format (`data/city.txt`)

```
LOCATION <id> <name> <AREA|SHELTER>
ROAD     <id> <fromId> <toId> <weight>     # two-way
SHELTER  <id> <name> <locationId> <capacity>
```

## Rules the simulation follows

- Request priority = danger level, then vulnerable people, then group size; ties go to the earlier request.
- Each group goes to the nearest reachable shelter that can take the whole group.
- Road reports wait in a FIFO queue. Only a report the operator verifies changes the map.
- When a verified report blocks a road, every assigned group whose route used it is rerouted. If its shelter can no longer be reached, the group is moved to another shelter, or marked FAILED if no shelter is reachable.

## Team (DSCPP-III-2026-T189)

- Piyush Koli (lead)
- Ayushi Rawat
- Jatin Bohra
