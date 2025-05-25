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
	std::string m_name;
	unsigned int m_points;
	bool m_isSaving;
	bool m_wasSaved;
};