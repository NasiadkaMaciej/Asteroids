#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>

#define GAME_VERSION 111

enum eStates : char // Game States
{
	playState,
	menuState,
	gameoverState,
	settingsState,
	saveScreenState,
	leaderBoardState
};
enum eSizes : char // Asteroids size
{
	BIG,
	MEDIUM,
	SMALL
};

const static float degToRad = M_PI / 180;

extern sf::VideoMode desktopMode;
extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Text text;
extern float screenScale;

// for storing game values, in future some timers and upgrades
struct GameValues
{
public:
	int bigAsteroids = 4;		// starting game with 6 asteroids (when starting 2 more are added)
	int roundNum = 0;			// round number (when starting game 1 is added)
	int powerUpRestore = 15000; // time to restore power ups
	int UFORestore = 30000;		// UFO exist and respawn time
};

struct Timer
{
	std::chrono::_V2::system_clock::time_point start, end;
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	unsigned int ms()
	{
		end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	}
	// ~Timer()
	// {
	// 	end = std::chrono::high_resolution_clock::now();
	// 	unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	// 	printf("%ims", duration);
	// }
};

struct GameTime
{
	Timer timer;
	float Shoot = 0, PowerUp = 0, Menu = 0, Move = 0, UFO = 0, ufoShoot = 0;
	void update()
	{
		Shoot += timer.ms();
		PowerUp += timer.ms();
		Menu += timer.ms();
		Move += timer.ms();
		UFO += timer.ms();
		ufoShoot += timer.ms();
		timer.reset();
	}
};

bool checkVersion();
extern sf::Text newVersion;

struct GameSettings
{
	GameSettings()
	{
		loadSettings();
	}
	// Use map instead
	unsigned int frames = 60, resX = sf::VideoMode::getFullscreenModes().front().width, resY = sf::VideoMode::getFullscreenModes().front().height, antialias = 0;
	bool vsync = true, fs = true, sfx = true, music = true, background = true;
	void loadSettings();
	void saveSettings();
	int translateFS();
	bool checkRes();
	std::vector<sf::VideoMode> availRes;
	void reloadWindow();
};

extern GameValues *gameVal;
extern GameTime *delta;
extern GameSettings gameSettings;

extern char activeState;
extern void setState(eStates state);

extern bool loadBase();