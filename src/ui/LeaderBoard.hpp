#pragma once
#include "GameOver.hpp"

class LeaderBoard : public GameOver {
  public:
	LeaderBoard(int entriesCount, std::string entries[]);
	void click() override;
	void setScore();
};