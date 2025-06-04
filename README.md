# Asteroids

![Game screenshot](https://nasiadka.pl/project/asteroids/Asteroids.png)

Asteroids game clone. Written in C++ with SFML.  
Destroy as many asteroids as you can with your spaceship's missiles, but be careful, they come from all sides, at different speeds and, when destroyed, they break into more, even faster and more dangerous fragments. Be cautious! Once in a while, a mysterious evil UFO shows up and starts shooting to you.
Each level ends after destroying all visible asteroids. After that, you get into the next level, which has 2 more asteroids than the previous one.
You start the game with 3 lives, after you lose one, you enter idle state, in which asteroids cannot hurt you, but you can't shoot them. To continue the game, simply move your ship. When you lose all your lives you can save your score and send it to [Asteroids online leaderboard](https://nasiadka.pl/asteroids/)  
## Steering
### Keyboard
Arrows or WASD - Move  
Spacebar - Shoot  
N - New game  
ESC - Entering and exiting menu  
Enter - Choosing menu entry  
F11 - Toggling fullscreen  
M - Toggle music at any time
### Gamepad (PS3)
Left joystick - Rotate  
R2 - Thrust  
X - Shoot  
Triangle - New game  
START - Entering menu  
Arrows - Navigate in menu
O - Exiting menu  
X - Choosing menu entry  
L1 - Toggling fullscreen  
R1 - Toggle music at any time
## Building
You need SFML, libcurl (with ssl) and CMAKE installed. Use build.sh script or command below.  
Cmake build works on Linux. I have some problems on Windows due some libraries in vcpkg.  
````shell
g++ GameSettings.cpp Collision.cpp main.cpp -o asteroids -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lcurl
````   
Contributors are welcome!
