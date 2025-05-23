#include "Settings.hpp"
#include "../utils/sounds.hpp"
#include "MenuUtils.hpp"

extern sf::RenderWindow window;
extern void setState(eStates state);

Settings::Settings(int entriesCount, std::string entries[])
  : Menu(entriesCount, entries) {}

void Settings::click() {
	switch (activeEntry) {
	case 0:
		switchRefreshRate();
		break;
	case 1:
		toggleVsync();
		break;
	case 2:
		toggleFS();
		break;
	case 3:
		toggleSFX();
		break;
	case 4:
		toggleMusic();
		break;
	case 5:
		toggleBackground();
		break;
	case 6:
		switchResolution();
		break;
	case 7:
		switchAA();
		break;
	case 8:
		setState(menuState);
		break;
	}
	move(0);
	gameSettings.saveSettings();
}

void Settings::switchRefreshRate() {
	switch (gameSettings.frames) {
	case 60:
		gameSettings.frames = 75;
		break;
	case 75:
		gameSettings.frames = 120;
		break;
	case 120:
		gameSettings.frames = 144;
		break;
	case 144:
		gameSettings.frames = 165;
		break;
	case 165:
		gameSettings.frames = 500;
		break;
	case 500:
		gameSettings.frames = 1000;
		break;
	case 1000:
		gameSettings.frames = 60;
		break;
	}
	window.setFramerateLimit(gameSettings.frames);
	entries[0] = "Frame rate limit: " + std::to_string(gameSettings.frames);
}

void Settings::toggleVsync() {
	gameSettings.vsync = !gameSettings.vsync;
	window.setVerticalSyncEnabled(gameSettings.vsync);
	entries[1] = "VSync: " + returnBool(gameSettings.vsync);
}

void Settings::toggleFS() {
	gameSettings.fs = !gameSettings.fs;
	gameSettings.reloadWindow();
	entries[2] = "Fullscreen: " + returnBool(gameSettings.fs);
}

void Settings::toggleSFX() {
	gameSettings.sfx = !gameSettings.sfx;
	entries[3] = "Sound: " + returnBool(gameSettings.sfx);
}

void Settings::toggleMusic() {
	gameSettings.music = !gameSettings.music;
	entries[4] = "Music: " + returnBool(gameSettings.music);
	playMusic();
}

void Settings::toggleBackground() {
	gameSettings.background = !gameSettings.background;
	entries[5] = "Background: " + returnBool(gameSettings.background);
}

void Settings::switchResolution() {
	if (gameSettings.availRes.end()->size.x == gameSettings.resX &&
		gameSettings.availRes.end()->size.y == gameSettings.resY) {
		gameSettings.resX = gameSettings.availRes.begin()->size.x;
		gameSettings.resY = gameSettings.availRes.begin()->size.y;
	} else
		for (auto it = gameSettings.availRes.begin(); it != gameSettings.availRes.end(); it++)
			if (it->size.x == gameSettings.resX && it->size.y == gameSettings.resY) {
				it++;
				gameSettings.resX = it->size.x;
				gameSettings.resY = it->size.y;
				break;
			}
	entries[6] = "Resolution: " + std::to_string(gameSettings.resX) + "x" + std::to_string(gameSettings.resY);
}

void Settings::switchAA() {
	if (gameSettings.antialias == 0)
		gameSettings.antialias = 2;
	else if (gameSettings.antialias < 16)
		gameSettings.antialias = gameSettings.antialias * 2;
	else if (gameSettings.antialias == 16)
		gameSettings.antialias = 0;

	entries[7] = "Antialiasing: " + std::to_string(gameSettings.antialias);
	gameSettings.reloadWindow();
}