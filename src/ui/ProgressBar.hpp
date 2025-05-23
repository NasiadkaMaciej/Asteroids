#pragma once
#include "../utils/GameSettings.hpp"
#include <SFML/Graphics.hpp>

struct ProgressBar {
  public:
	float fullSize;
	sf::RectangleShape pg;
	float objectSize;
	float numOfObjects;
	float removedObjects = 0;
	float height;

	ProgressBar(float HEIGHT);
	void retractPoint();
	void update();
	void reset();
};

// External mute function for controlling settings during play
void mute(class Settings* s);

extern ProgressBar progressBar, placeholder;