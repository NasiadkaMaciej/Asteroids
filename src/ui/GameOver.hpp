#pragma once
#include "Menu.hpp"

class GameOver : public Menu {
  public:
	GameOver();
	void setScore(int points);
};