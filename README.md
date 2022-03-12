# Asteroids

![Game screenshot](https://maciej.ml/projects/Asteroids/Asteroids.png)

Alpha phaze Asteroids game clone. Written in C++ with SFML.
The level ends after destroying all visible asteroids.
The large asteroid breaks down into 2 medium asteroids, which in turn break down into 2 small ones.
Asteroids spawn at the edge of the screen.
Each level has 2 more asteroids than the previous one.

## Building
You need SFML installed
````shell
g++ Collision.cpp main.cpp -o asteroids -lsfml-graphics -lsfml-window -lsfml-system
````
## To-Do
* [ ] Code cleanup
* [x] Add asteroids and bullets
* [x] Check if game works on other systems and resolutions
* [ ] Menu and settings
* [ ] Progress bar
* [ ] Leaderboard
