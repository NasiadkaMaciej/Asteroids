#include <SFML/Audio.hpp>
#include <iostream>

sf::SoundBuffer laserBuffer, destroyBuffer, deathBuffer;
sf::Sound laserSound, destroySound, deathSound;

void loadSounds()
{
    laserSound.setVolume(50);
    destroySound.setVolume(50);
    deathSound.setVolume(50);
    std::string dir = "audio/";
    if (!laserBuffer.loadFromFile(dir + "laser.wav") ||
        !destroyBuffer.loadFromFile("audio/destroy.wav") ||
        !deathBuffer.loadFromFile("audio/death.wav"))
    {
        std::cout << "Error loading sounds\n";
    }
    else
    {
        laserSound.setBuffer(laserBuffer);
        destroySound.setBuffer(destroyBuffer);
        deathSound.setBuffer(deathBuffer);
    }
}
void playSound(sf::Sound *sound){
    if(!isMute)
        sound->play();
}