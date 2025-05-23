#pragma once
#include <SFML/Graphics.hpp>

namespace CONTROL {
bool isESC();
bool isReset();
bool isFS();
bool isRight();
bool isLeft();
bool isThrust();
bool isUP();
bool isDOWN();
bool isSpace();
bool isEnter();
bool isClick();
bool mute();
float getAxisPos(sf::Joystick::Axis axis);
}