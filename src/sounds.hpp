#include <SFML/Audio.hpp>
#include <iostream>

sf::SoundBuffer laserBuffer, destroyBuffer, deathBuffer, menuBuffer;
sf::Sound laserSound, destroySound, deathSound, menuSound;
sf::Music music;

bool loadSounds()
{
    laserSound.setVolume(50);
    destroySound.setVolume(50);
    deathSound.setVolume(50);
    std::string dir = "audio/";

    if (!laserBuffer.loadFromFile(dir + "laser.wav") ||
        !destroyBuffer.loadFromFile(dir + "destroy.wav") ||
        !deathBuffer.loadFromFile(dir + "death.wav") ||
        !menuBuffer.loadFromFile(dir + "menu.wav") ||
        !music.openFromFile(dir + "music.wav"))
    {
        std::cout << "Error loading sounds\n";
        return false;
    }
    laserSound.setBuffer(laserBuffer);
    destroySound.setBuffer(destroyBuffer);
    deathSound.setBuffer(deathBuffer);
    menuSound.setBuffer(menuBuffer);
    menuSound.setLoop(false);
    music.setLoop(true);
    return true;
}
void playSound(sf::Sound *sound)
{
    if (gameSettings.sfx)
        sound->play();
}
void playMusic()
{
    if (gameSettings.music)
        music.play();
}