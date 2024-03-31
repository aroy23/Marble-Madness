# Marble Madness

## Game Overview

In Marble Madness, the player navigates through a series of robot-infested mazes to gather valuable crystals. After collecting all crystals within a maze, an exit is revealed, allowing the player to advance to the next maze. The game features various obstacles, including robots, marbles, pits, and goodies.

## Game Details

- **Levels**: Multiple levels starting from level 0, each with its own maze.
- **Objective**: Gather all blue crystals to reveal the exit and advance to the next level.
- **Gameplay**: Move avatar with arrow keys, collect goodies, shoot pea cannon, and avoid or destroy robots.
- **Goodies**: Extra life, restore health, and ammo.
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
- **ThiefBot**: Wander, pick up goodies, shoot at player.
- **Factories**: Produce ThiefBots.
- **Goodies**: Grant bonuses when collected.

## Conclusion

Marble Madness is an exciting game where players must navigate challenging mazes filled with obstacles and enemies. With strategic movement and precise shooting, players can conquer each level and emerge victorious. Are you ready to take on the challenge and lead yourself to victory?
