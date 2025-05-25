#include "Settings.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"
#include "MenuUtils.hpp"

extern void setState(eStates state);

Settings::Settings()
  : Menu() {
	addItem("Frame rate limit: " + std::to_string(gameSettings.frames), [this]() { switchRefreshRate(); });

	addItem("VSync: " + returnBool(gameSettings.vsync), [this]() { toggleVsync(); });

	addItem("Fullscreen: " + returnBool(gameSettings.fs), [this]() { toggleFS(); });

	addItem("Sound: " + returnBool(gameSettings.sfx), [this]() { toggleSFX(); });

	addItem("Music: " + returnBool(gameSettings.music), [this]() { toggleMusic(); });

	addItem("Background: " + returnBool(gameSettings.background), [this]() { toggleBackground(); });

	addItem("Resolution: " + std::to_string(gameSettings.resX) + "x" + std::to_string(gameSettings.resY),
			[this]() { switchResolution(); });

	addItem("Antialiasing: " + std::to_string(gameSettings.antialias), [this]() { switchAA(); });

	addItem("Menu", []() { setState(menuState); });
}

void Settings::update() {
	Menu::update();

	if (CONTROL::mute() && delta->Menu > 300) {
		toggleMusic();
		delta->Menu = 0;
		gameSettings.saveSettings();
	}
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

	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::toggleVsync() {
	gameSettings.vsync = !gameSettings.vsync;
	gameSettings.reloadWindow();
	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::toggleFS() {
	gameSettings.fs = !gameSettings.fs;
	gameSettings.reloadWindow();
	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::toggleSFX() {
	gameSettings.sfx = !gameSettings.sfx;
	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::toggleMusic() {
	gameSettings.music = !gameSettings.music;
	updateMenuTexts();
	playMusic();
	gameSettings.saveSettings();
}

void Settings::toggleBackground() {
	gameSettings.background = !gameSettings.background;
	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::switchResolution() {
	// Cycle through available resolutions
	if (gameSettings.availRes.empty()) return;

	for (size_t i = 0; i < gameSettings.availRes.size(); i++) {
		if (gameSettings.availRes[i].size.x == gameSettings.resX &&
			gameSettings.availRes[i].size.y == gameSettings.resY) {

			i = (i + 1) % gameSettings.availRes.size();
			gameSettings.resX = gameSettings.availRes[i].size.x;
			gameSettings.resY = gameSettings.availRes[i].size.y;
			break;
		}
	}

	gameSettings.reloadWindow();
	updateMenuTexts();
	updateItemPositions();
	gameSettings.saveSettings();
}

void Settings::switchAA() {
	switch (gameSettings.antialias) {
	case 0:
		gameSettings.antialias = 2;
		break;
	case 2:
		gameSettings.antialias = 4;
		break;
	case 4:
		gameSettings.antialias = 8;
		break;
	case 8:
		gameSettings.antialias = 16;
		break;
	case 16:
		gameSettings.antialias = 0;
		break;
	}

	gameSettings.reloadWindow();
	updateMenuTexts();
	gameSettings.saveSettings();
}

void Settings::updateMenuTexts() {
	if (getItemCount() >= 8) {
		getItem(0)->setText("Frame rate limit: " + std::to_string(gameSettings.frames));
		getItem(1)->setText("VSync: " + returnBool(gameSettings.vsync));
		getItem(2)->setText("Fullscreen: " + returnBool(gameSettings.fs));
		getItem(3)->setText("Sound: " + returnBool(gameSettings.sfx));
		getItem(4)->setText("Music: " + returnBool(gameSettings.music));
		getItem(5)->setText("Background: " + returnBool(gameSettings.background));
		getItem(6)->setText("Resolution: " + std::to_string(gameSettings.resX) + "x" +
							std::to_string(gameSettings.resY));
		getItem(7)->setText("Antialiasing: " + std::to_string(gameSettings.antialias));
	}
}