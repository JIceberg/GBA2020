# Gamer

**ARE *YOU* AN EPIC GAMER?**

-----------------

## About
Gamer is a single-platform, increasing-difficulty, dodge-and-weave game for the GBA.
Dodge and weave refers to gameplay where the user is asked to avoid collision with
moving objects on the screen. For "Gamer," this is the core objective; so, the goal
is to avoid making contact with these moving objects.

## Game States
The game starts on the "START" screen, where you are asked to press the start button
to begin the game. This takes you into the "PLAY" screen, which only consists of a
single platform for the gameplay. If you press start at any point during this
game state, the game will pause: everything on the screen freezes and no motion occurs.
Pressing the start button once more will resume the gameplay.

On the lose and win screens, press enter to play again.

In any game state, you can press the select key to return to the
"START" screen, which restarts the entire program (all progress will be lost).
You will be prompted if you are sure you want to reset. You can either press
start or select as "Yes" or "No" respectively, i.e. the start button will
reset the program and the select button will return you to the previous screen.

---
<p style="text-align: center;">
<b>NOTE</b>
<br/>
When you pause and then press select, you are prompted like usual.
However, when you return to the previous screen, the gameplay starts
back up. You will need to press enter again.
</p>

---

## Gameplay
When the game starts, your character will start to fall down. Throughout the game,
when there is no button input, the player's character will fall at a constant speed.
You can control the direction of motion by holding down the arrow keys. Pressing
down will increase the speed of descent down the screen. The user's movement
is constrained to a certain area of the screen. Any attempt to leave the screen will
be thwarted.

Avoid the white squares. Enemies (the squares) will spawn at random locations on
the screen. There will be a delay period before they begin movement. During this time,
the enemies are red. This means you can collide with them without losing during this period.
Enemies can move diagonally, horizontally, or vertically. Enemies that move vertically
will spawn at any column but only at the top or bottom of the screen. Squares that move
horizontally will spawn at any row but only on the left or right of the screen. Enemies
that can move diagonally will spawn at any column but always at the bottom of the screen.

There is a 1 in 7 chance of an enemy spawning with the ability to rebound, where upon collision
with the edge of the screen they will bounce off and move in the opposite direction.
The number of rebounds performed is dependent upon the enemy's speed. If the enemy cannot
rebound, they will respawn with a random movement type in the constrained spawn position.
Again, there is a delay between being spawned and beginning movement.

Sometimes, portals appear!

### Strategy
Understanding how the behavior of the enemies works is key to victory.

If an enemy spawns...
- at the top, it can move vertical or horizontal
- centered column-wise, it can move vertical or diagonal
- centered row-wise, it can ONLY move horizontal
- at the bottom, it can have any movement pattern

Beating the game (a score of 7500) will require you to quickly account for these characteristics
in the brief period during which an enemy is stationary. Combine this with accounting for the locations
of the other enemies currently on the screen and the possibility of rebound, and
the strategy is in place.
