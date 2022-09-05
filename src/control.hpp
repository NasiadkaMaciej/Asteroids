#pragma once
#include <SFML/Graphics.hpp>

namespace CONTROL
{
    bool isESC()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1) || sf::Joystick::isButtonPressed(0, 9);
    }
    bool isReset()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::N) || sf::Joystick::isButtonPressed(0, 2);
    }
    bool isFS()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F11) || sf::Joystick::isButtonPressed(0, 4);
    }
    bool isRight()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 0;
    }
    bool isLeft()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < 0;
    }
    bool isThrust()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) > 0 || sf::Joystick::isButtonPressed(0, 13);
    }
    bool isUP()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::isButtonPressed(0, 13);
    }
    bool isDOWN()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::isButtonPressed(0, 14);
    }
    bool isSpace()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 0);
    }
    bool isEnter()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Joystick::isButtonPressed(0, 0);
    }
    bool isClick()
    {
        return sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }
    bool mute()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::M) || sf::Joystick::isButtonPressed(0, 5);
    }
};