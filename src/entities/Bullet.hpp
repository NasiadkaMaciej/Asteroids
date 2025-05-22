#pragma once

#include "Entity.hpp"
#include "utils.hpp"

class Bullet : public Entity {
  public:
	char type() { return _BULLET; }
	Bullet(float X, float Y, sf::Angle ANGLE, sf::Texture* TEXTURE, Scale s)
	  : Entity(X, Y, 0, 0, ANGLE, TEXTURE) {
		sprite.scale({ s.x, s.y });
		lifes = 1;
	};
	void update() {
		sf::Angle angle_rad = angle; // convert angle in degrees to sf::Angle

		// Calculate x_speed and y_speed using sf::Angle
		x_speed = (cos(angle_rad.asRadians()) * delta->Move / 5) * speedScale;
		y_speed = (sin(angle_rad.asRadians()) * delta->Move / 5) * speedScale;
		x += x_speed;
		y += y_speed;
		if (x > gameSettings.resX || x < 0 || y > gameSettings.resY || y < 0) {
			life = false;
			lifes--;
		}
	}
};