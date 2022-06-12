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

extern sf::Font font;
extern sf::Text text;

extern float scale;

struct GameValues
{
public:
  int bigAsteroids = 4;
  int roundNum = 0;
  GameValues()
  {
    // game start values;
    bigAsteroids = 4; // when generating, 2 more are created
    roundNum = 0;     // when starting, 1 is added
  }
  void reset(); // Change to making new object
};

extern GameValues gameVal;

struct GameTime
{
public:
  sf::Clock Clock;
  sf::Time Time;
  float PowerUp, Menu, Move;
  void reset()
  {
    PowerUp = 0;
    Menu = 0;
    Move = 0;
  }
};

extern sf::VideoMode desktopMode;
extern sf::RenderWindow window;

// to array?
extern bool isPlaying, isMenu, isGameOver, isSettings, isSaveScreen, isLeaderBoard;

struct GameSettings
{
public:
  GameSettings()
  {
    loadSettings();
  }
  int frames = 60;
  bool vsync = true, fs = true, sfx = true, music = true, background = true;
  void loadSettings();
  void saveSettings();
};
int translateFS(int fs);

extern GameSettings gameSettings;
extern GameTime delta;

extern bool loadBase();
extern void setStates(bool state);

extern void setState(int state);