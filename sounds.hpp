#include <SFML/Audio.hpp>
#include <iostream>

sf::SoundBuffer laserBuffer, deathBuffer;
sf::Sound laserSound, deathSound;

void loadSounds()
{
    std::string dir = "audio/";
    if (!laserBuffer.loadFromFile(dir + "laser.wav") ||
        !deathBuffer.loadFromFile("audio/death.wav"))
    {
        std::cout << "Error loading sounds\n";
    }
    else
    {
        laserSound.setBuffer(laserBuffer);
        deathSound.setBuffer(deathBuffer);
    }
}