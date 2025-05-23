#include "control.hpp"

namespace CONTROL {

bool isESC() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Joystick::isButtonPressed(0, 1) ||
		   sf::Joystick::isButtonPressed(0, 9);
}

bool isReset() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N) || sf::Joystick::isButtonPressed(0, 2);
}

bool isFS() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11) || sf::Joystick::isButtonPressed(0, 4);
}

bool isRight() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		   sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 0;
}

bool isLeft() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
		   sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < 0;
}

bool isThrust() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
		   sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) > 0 || sf::Joystick::isButtonPressed(0, 13);
}

bool isUP() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
		   sf::Joystick::isButtonPressed(0, 13);
}

bool isDOWN() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
		   sf::Joystick::isButtonPressed(0, 14);
}

bool isSpace() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Joystick::isButtonPressed(0, 0);
}

bool isEnter() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || sf::Joystick::isButtonPressed(0, 0);
}

bool isClick() {
	return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

bool mute() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M) || sf::Joystick::isButtonPressed(0, 5);
}

float getAxisPos(sf::Joystick::Axis axis) {
	return sf::Joystick::getAxisPosition(0, axis) / 100.f;
}

}