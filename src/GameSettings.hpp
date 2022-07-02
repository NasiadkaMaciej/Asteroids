#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>

// states
#define playState 0
#define menuState 1
#define gameoverState 2
#define settingsState 3
#define saveScreenState 4
#define leaderBoardState 5

#define BIG 0
#define MEDIUM 1
#define SMALL 2

extern float degToRad;

extern sf::VideoMode desktopMode;
extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Text text;

extern float scale;

// to array or struct
extern bool isPlaying, isMenu, isGameOver, isSettings, isSaveScreen, isLeaderBoard;
// for storing game values, in future some timers and upgrades
struct GameValues
{
public:
  int bigAsteroids = 4;       // starting game with 6 asteroids (when starting 2 more are added)
  int roundNum = 0;           // round number (when starting game 1 is added)
  int powerUpRestore = 10000; // time to restore power ups
  int UFORestore = 30000;     // UFO exist and respawn time
};

struct GameTime
{
public:
  sf::Clock Clock;
  sf::Time Time;
  float Shoot = 0, PowerUp = 0, Menu = 0, Move = 0, UFO = 0, ufoShoot;
  void gameUpdate()
  {
    PowerUp += Time.asMilliseconds();
    Move += Time.asMilliseconds();
    UFO += Time.asMilliseconds();
    Shoot += Time.asMilliseconds();
    ufoShoot += Time.asMilliseconds();
  }
};

struct GameSettings
{
public:
  GameSettings()
  {
    loadSettings();
  }
  // Default values
  int frames = 60, resX = sf::VideoMode::getFullscreenModes().front().width, resY = sf::VideoMode::getFullscreenModes().front().height;
  bool vsync = true, fs = true, sfx = true, music = true, background = true;
  void loadSettings();
  void saveSettings();
  int translateFS();
  bool checkRes();
};

extern GameValues *gameVal;
extern GameSettings gameSettings;
extern GameTime *delta;

extern bool loadBase();
extern void setStates(bool state);

extern void setState(int state);