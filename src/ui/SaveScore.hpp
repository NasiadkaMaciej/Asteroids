#pragma once
#include "Menu.hpp"

class SaveScore : public Menu {
  public:
	std::string name;
	unsigned int points;
	bool isSaving = true, wasSaved = false;

	SaveScore(int entriesCount, std::string entries[]);
	void click() override;
	void setScore(int points);
	void saveScore();
	void reset();
};