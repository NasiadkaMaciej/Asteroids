# Asteroids

![Game screenshot](https://maciej.ml/projects/Asteroids/Asteroids.png)

Asteroids game clone. Written in C++ with SFML.  
Destroy as many asteroids as you can with your spaceship's missiles, but be careful, they come from all sides, at different speeds and, when destroyed, they break into more, even faster and more dangerous fragments.
Each level ends after destroying all visible asteroids. After that, you get into the next level, which has 2 more asteroids than the previous one.
You start the game with 3 lives, after you lose one, you enter idle state, in which asteroids cannot hurt you, but you can't shoot them. To continue the game, simply move your ship. When you loose all your lives you can save your score and send it to [Asteroids online leaderboard](https://maciej.ml/Asteroids/)  
Use arrows to move, spacebar to shoot, N for new game, ESC for entering and exiting menu and Enter for choosing menu entry.

## Building
You need SFML installed
````shell
g++ Collision.cpp main.cpp -o asteroids -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lcurl
````
## To-Do
* [ ] Code cleanup
* [ ] Make more functional
* [x] Repair time problem after starting new game
* [x] Add asteroids and bullets
* [x] Check if game works on other systems and resolutions
* [x] Menu and settings
* [x] Game over screen
* [x] Clickable menu options
* [x] Progress bar
* [x] Leaderboard
* [x] Online leaderboard
* [x] Toggling fullscreen
* [ ] Saving settings
* [ ] Android Support
* [ ] Make executables for Windows and Linux
* [ ] Toggle plain black background and stars background
* [ ] Repair setting cursor to first position
* [ ] Add delta time
* [ ] Change method of inputting name
* [x] Add link to online scoreboard
* [ ] Integrate local and online scoreboard
* [ ] Add user agreement to send data
* [ ] Add info about update
* [x] Add sound
* [x] Repair fullscreen and screen frequency settings
* [ ] Add sound mute toggle
* [ ] Add timestamp to scores
* [ ] Fair play adjustment for different resolutions
* [ ] Add multiplayer
* [ ] Scrolling menus
