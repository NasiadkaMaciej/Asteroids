#pragma once
#include "../utils/GameSettings.hpp"
#include "Menu.hpp"

class Settings : public Menu {
  public:
	Settings(int entriesCount, std::string entries[]);
	void click() override;

	void switchRefreshRate();
	void toggleVsync();
	void toggleFS();
	void toggleSFX();
	void toggleMusic();
	void toggleBackground();
	void switchResolution();
	void switchAA();
};
