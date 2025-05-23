#include "Bullet.hpp"
#include "../utils/GameSettings.hpp"

char Bullet::type() {
	return _BULLET;
}

Bullet::Bullet(float X, float Y, sf::Angle ANGLE, sf::Texture* TEXTURE, Scale s)
  : Entity(X, Y, 0, 0, ANGLE, TEXTURE) {
	sprite.scale({ s.x * 2, s.y * 2 });
	lifes = 1;

	x = X;
	y = Y;
	x_speed = std::cos(angle.asRadians()) * bulletSpeed;
	y_speed = std::sin(angle.asRadians()) * bulletSpeed;
	sprite.setRotation(ANGLE + sf::degrees(90.f));
}

void Bullet::update() {
	x += x_speed * delta->Move / 10;
	y += y_speed * delta->Move / 10;
	if (x > gameSettings.resX || x < 0 || y > gameSettings.resY || y < 0) {
		life = false;
		lifes--;
	}
}