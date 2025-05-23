#pragma once
#include "Menu.hpp"

class GameOver : public Menu {
  public:
	GameOver(int entriesCount, std::string entries[]);
	void click() override;
	void setScore(int points);
};