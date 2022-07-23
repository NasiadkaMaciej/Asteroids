# Asteroids

![Game screenshot](https://maciej.ml/projects/Asteroids/Asteroids.png?)

Asteroids game clone. Written in C++ with SFML.  
Destroy as many asteroids as you can with your spaceship's missiles, but be careful, they come from all sides, at different speeds and, when destroyed, they break into more, even faster and more dangerous fragments. Be cautious! Once in a while, a mysterious UFO shows up and starts shooting to you.
Each level ends after destroying all visible asteroids. After that, you get into the next level, which has 2 more asteroids than the previous one.
You start the game with 3 lives, after you lose one, you enter idle state, in which asteroids cannot hurt you, but you can't shoot them. To continue the game, simply move your ship. When you loose all your lives you can save your score and send it to [Asteroids online leaderboard](https://maciej.ml/Asteroids/)  
Arrows or WASD to move  
Spacebar to shoot  
N for new game  
ESC for entering and exiting menu  
Enter for choosing menu entry  
F11 for toggling fullscreen (game restart recommended)  

## Building
You need SFML, libcurl and CMAKE installed. Use build.sh script or command below.  
Cmake works on Linux. I have some problems on Windows due some libraries in vcpkg.
````shell
g++ Collision.cpp main.cpp -o asteroids -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lcurl
````
## To-Do
* [ ] Code cleanup
* [x] Make more functional
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
* [x] Saving settings
* [ ] Android Support
* [x] Make executables for Windows and Linux
* [x] Toggle plain black background and stars background
* [x] Repair setting cursor to first position
* [x] Add delta time
* [x] Add link to online scoreboard
* [ ] Integrate local and online scoreboard
* [ ] Add user agreement to send data
* [ ] Add info about update
* [x] Add sound
* [x] Repair fullscreen and screen frequency settings
* [x] Add sound mute toggle
* [x] Add timestamp to scores
* [x] Fair play adjustment for different resolutions
* [ ] Add multiplayer
* [ ] Scrolling menus
* [ ] Auto maximize and focus game if not in fullscreen
* [ ] Audio volume settings
* [ ] Progressbar improvement
* [ ] Achievments
* [ ] Upgrades
* [ ] Cosmetic changes
* [ ] Reformat readme
* [ ] More threads
* [ ] Animated menu background
* [ ] Async music toggle
* [ ] All entities to one list
* [ ] Less for loops
* [ ] More optimization
* [ ] One for loop and type in class name
* [x] Choose resolution in game