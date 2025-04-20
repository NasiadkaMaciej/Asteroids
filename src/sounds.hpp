#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>

extern sf::SoundBuffer laserBuffer, destroyBuffer, deathBuffer, menuBuffer, ufoLaserBuffer;
extern sf::Sound laserSound, destroySound, deathSound, menuSound, ufoLaserSound;
extern sf::Music music;

bool loadSounds(const std::filesystem::path& dir = "audio/");
void playSound(sf::Sound* sound);
void playMusic();