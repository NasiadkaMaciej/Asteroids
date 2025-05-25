#pragma once
#include "Menu.hpp"
#include <SFML/System/Utf.hpp>

class SaveScore : public Menu {
  public:
	SaveScore();

	void update() override;
	void show() override;
	void onTextEntered(char character);
	void setScore(int points);
	void saveScore();
	void reset();

  private:
	std::string name;
	unsigned int points;
	bool isSaving;
	bool wasSaved;
};