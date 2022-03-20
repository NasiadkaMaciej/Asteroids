# Asteroids

![Game screenshot](https://maciej.ml/projects/Asteroids/Asteroids.png)

Alpha phaze Asteroids game clone. Written in C++ with SFML.
The level ends after destroying all visible asteroids.
The large asteroid breaks down into 2 medium asteroids, which in turn break down into 2 small ones.
Asteroids spawn at the edge of the screen.
Each level has 2 more asteroids than the previous one.
Use arrows to move, spacebar to shoot, N for new game, ESC for entering and exiting menu and Enter for choosing menu entry

## Building
You need SFML installed
````shell
g++ Collision.cpp main.cpp -o asteroids -lsfml-graphics -lsfml-window -lsfml-system
````
## To-Do
* [ ] Code cleanup
* [ ] Make more functional
* [x] Repair time problem after starting new game
* [x] Add asteroids and bullets
* [x] Check if game works on other systems and resolutions
* [ ] Menu and settings
* [ ] Game over screen
* [ ] Clickable menu options
* [ ] Progress bar
* [ ] Leaderboard
