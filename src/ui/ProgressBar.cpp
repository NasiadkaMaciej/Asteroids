#include "ProgressBar.hpp"
#include "../utils/control.hpp"
#include "Settings.hpp"

extern sf::RenderWindow window;
extern GameValues* gameVal;
extern GameTime* delta;

ProgressBar::ProgressBar(float HEIGHT)
  : height(HEIGHT)
  , fullSize(gameSettings.resX / 2) {
	pg.setFillColor(sf::Color::Black);
	pg.setOutlineThickness(1);
	pg.setOutlineColor(sf::Color::White);
}

void ProgressBar::retractPoint() {
	numOfObjects--;
	removedObjects += objectSize;
}

void ProgressBar::update() {
	pg.setSize(sf::Vector2f(fullSize - removedObjects, height));
	pg.setOrigin(pg.getSize() / 2.f);
	pg.setPosition({ window.getView().getCenter().x, window.getView().getCenter().y * 1.95f });
}

void ProgressBar::reset() {
	removedObjects = 0;
	numOfObjects = gameVal->bigAsteroids + gameVal->bigAsteroids * 2 + gameVal->bigAsteroids * 4;
	objectSize = fullSize / numOfObjects;
}

ProgressBar progressBar(15), placeholder(0);

void mute(Settings* s) {
	if (CONTROL::mute() && delta->Menu > 300) {
		s->toggleMusic();
		delta->Menu = 0;
		gameSettings.saveSettings();
	}
}