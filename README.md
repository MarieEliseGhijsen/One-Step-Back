# One Step Back
### Every move matters. Sometimes, taking one step back makes all the difference.
A 2D tile-based puzzle adventure game developed in C++ using [SFML](https://www.sfml-dev.org/).

Players must think strategically about every movement, as enemies pursue them. Rewinding allows players to undo previous movements, providing an opportunity to rethink their strategy and escape dangerous situations.

The objective is to complete all five levels while collecting coins, avoiding enemies, and achieving the highest possible score.

## Tech Used
- C++
- SFML
- Breadth-First Search (BFS) - Enemy pathfinding algorithm
- File Handling - Loading level data and saving leaderboard scores

## Controls
- ↑       Move up
- ↓       Move down
- ←       Move left
- →       Move right
- R       Rewind the previous four movements
- P       Pause the game
- Mouse   Navigate menus and select options

## Scoring System
- Complete a level            100
- Collect a coin              50
- Collect all three coins     200 bonus
- Time bonus                  Up to 300

The time bonus decreases as the time taken to complete a level increases.
The final leaderboard score is calculated from the accumulated scores across all five levels.

## Enemy AI
Enemies use the Breadth-First Search (BFS) algorithm to find the shortest available path towards the player's current position.