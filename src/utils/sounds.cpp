#include "sounds.hpp"
#include "GameSettings.hpp"

// define buffers first
sf::SoundBuffer laserBuffer, destroyBuffer, deathBuffer, menuBuffer, ufoLaserBuffer;
// then sounds, each bound to its buffer
sf::Sound laserSound{ laserBuffer }, destroySound{ destroyBuffer }, deathSound{ deathBuffer }, menuSound{ menuBuffer },
  ufoLaserSound{ ufoLaserBuffer };
sf::Music music;

bool loadSounds(const std::filesystem::path& dir) {
	if (!laserBuffer.loadFromFile(dir / "laser.wav")) return false;
	if (!destroyBuffer.loadFromFile(dir / "destroy.wav")) return false;
	if (!deathBuffer.loadFromFile(dir / "death.wav")) return false;
	if (!menuBuffer.loadFromFile(dir / "menu.wav")) return false;
	if (!ufoLaserBuffer.loadFromFile(dir / "ufoLaser.wav")) return false;

	if (!music.openFromFile(dir / "music.wav")) return false;
	music.setLooping(true);
	return true;
}

void playSound(sf::Sound* sound) {
	if (gameSettings.sfx) sound->play();
}

void playMusic() {
	if (gameSettings.music)
		music.play();
	else
		music.stop();
}