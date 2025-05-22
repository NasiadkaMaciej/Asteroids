#pragma once

#include "utils.hpp"

class Entity // general class for all existing entities
{
  public:
	virtual char type() { return -1; }
	float x, y, x_speed, y_speed;
	sf::Angle angle;
	bool life = true;
	int lifes = 0;
	sf::Sprite sprite;

	Entity(float X, float Y, float X_SPEED, float Y_SPEED, sf::Angle ANGLE, sf::Texture* TEXTURE)
	  : x(X)
	  , y(Y)
	  , x_speed(X_SPEED)
	  , y_speed(Y_SPEED)
	  , angle(ANGLE)
	  , sprite(*TEXTURE) {

		auto bounds = sprite.getGlobalBounds();
		sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
		sprite.scale(sf::Vector2f{ screenScale, screenScale });
	}

	const void draw(sf::RenderWindow& window) {
		sprite.setPosition(sf::Vector2f{ x, y });
		window.draw(sprite);
	}
	virtual void update() {};

	bool operator==(const sf::Texture* txt) const { return &sprite.getTexture() == txt; }
};