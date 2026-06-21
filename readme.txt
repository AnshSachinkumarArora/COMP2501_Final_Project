COMP2501 Winter 2026 Final Project
Name - Ansh Sachinkumar Arora
Student Number - 101218267
PROJECT REQUIREMENTS CHECKLIST -
1. Gameplay includes platfoming and melee combat. 
Finishing condition - Obtain key and defeat the boss at the end of the level to unlock the door.

2. 
2.1 There are 3 different enemy types. MOBs, TANKs, and BOSSes with different textures and animations.
2.2 The enemy types have unique HP, move speed, damage output, aggro range, and steering force while chasing the player.
Enemy behaviour is controlled by a state machine system with 5 unique states - Idle, Attack, Chase, Hurt, and Dead.
Each enemy has unique weapons and movesets.

3. The player has access to 2 melee weapons which can be switched with 'E'. They have different textures, attack range, and damage.
3.1 The knife the player starts with.
3.2 The player can pick up a sword that deals more damage and has more range.

4. 
4.1 The game has 2 collectible items - A sword and a key (needed for game completion).
4.2 The player can also platform to access a speed boost that vastly increases player speed for 5 seconds.

5.
5.1 All movement in the game is handled through transformations.
5.2 All characters move using physically based motion and the collectibles have a 'hover' motion that uses parametric motion.

6. The game uses circle-circle collision detection for player-object interactions (like picking up objects) and for combat.

7. 
7.1 The game world occupies a space larger than the screen and can be accessed by scrolling.
7.2 The larger map is constructed by tiling 2 unique backgrounds.

8.
8.1 Particle system 1 - Used when player hits non-boss enemies, on a hit enemies 'bleed' and the particles use gravity so they fall downwards.
4000 particles per system. Red color.
8.2 Particle system 2 - Used when player hits the boss enemy, on a hit the boss 'explodes' and releases sparks, there 
is no use of gravity and the particles get expelled in all directions. 1000 particles per system. Orange-ish color.

9. The UI displays current player health, time elapsed since start of game, and indicates whether key item has been picked up (which is needed to finish the level).

10. There are 2 advanced methods implemented - one of them is advanced pursuit (chase) steering behaviour. Each enemy has a different aggro range and pursues
at different speeds and has different acceleration.

11. 
11.1 This readme contains a description of all the requirements met in this project.
11.2 The code is thoroughly commented.

12
12.1 There is a second advanced method that is implemented. The player has collision resolution on hitting the extremeties of the map or jumping.
Hitting a surface with speed will cause the player to bounce off.

controls - 'A' and 'D' to move left and right. 'W' to jump. 'Space' to attack. 'E' to switch weapons.
build instructions - 
- mkdir bin
- cd bin
- cmake ..
- make
- ./FINAL_PROJECT