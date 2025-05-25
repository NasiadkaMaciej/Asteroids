#pragma once
#include "Menu.hpp"

class Settings : public Menu {
  public:
	Settings();
	void toggleMusic();
	void update() override;

  private:
	void switchRefreshRate();
	void toggleVsync();
	void toggleFS();
	void toggleSFX();
	void toggleBackground();
	void switchResolution();
	void switchAA();
	void updateMenuTexts();
};
