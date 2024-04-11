# Marble Madness

## Game Overview

In Marble Madness, the player navigates through a series of robot-infested mazes to gather valuable crystals. After collecting all crystals within a maze, an exit is revealed, allowing the player to advance to the next maze. The game features various obstacles, including robots, marbles, pits, and goodies.

- **Note**: XQuartz must be downloaded for this program to run: https://www.xquartz.org/
  
![image](https://github.com/aroy23/Marble-Madness/assets/83829580/a8dcb67b-49af-48b3-a2a7-db36d6eb5967)

## Game Details

- **Levels**: Multiple levels starting from level 0, each with its own maze.
- **Objective**: Gather all blue crystals to reveal the exit and advance to the next level.
- **Gameplay**: Move avatar with arrow keys, collect goodies, shoot pea cannon, and avoid or destroy robots.
- **Goodies**: Extra life, restore health, and ammo.
- **Marbles**: Can be pushed by player, not robots
- **Pit**: Obstacles for player and robots
- **Robots**: Horizontal/Vertical RageBots, Regular/Mean ThiefBots.
- **Factories**: Produce ThiefBots.
- **Exit**: Revealed after collecting all crystals in a level.

## Gameplay Mechanics

- **Controls**: Use arrow keys or WASD to move, space bar to shoot, and Escape to restart a level.
- **Scoring**: Earn points by collecting crystals, destroying robots, and completing levels quickly.
- **Lives**: Start with three lives; lose a life when health reaches zero.
- **Ticks**: Gameplay divided into ticks; each actor performs actions during ticks.
- **Phases**: Initialization, Gameplay, and Cleanup.

## Implementation Overview

- **Initialization**: Prepare game world, place actors.
- **Gameplay**: Actors perform actions during ticks.
- **Cleanup**: Handle end-of-level or game-over conditions.

## Actor Behavior

- **Player**: Move, shoot, collect goodies.
- **RageBot**: Move, shoot at player.
- **ThiefBot**: Wander, pick up goodies
- **Mean ThiefBot**: Wander, pick up goodies, shoot at player.
- **Factories**: Produce ThiefBots.
- **Pit** Obstacle that blocks a player, robots, but not peas
- **Marble** Can be pushed into a pit to make the pit disappear, can be destroyed
- **Goodies**: Grant bonuses when collected.

## Conclusion

Marble Madness is an exciting game where players must navigate challenging mazes filled with obstacles and enemies. With strategic movement and precise shooting, players can conquer each level and emerge victorious. Are you ready to take on the challenge and lead yourself to victory?

This Program Was Made For UCLA CS 32 Project 3 Taught by David Smallberg and Carey Nachenberg for the Winter 2024 Quarter
