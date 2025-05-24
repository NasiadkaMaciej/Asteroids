#include "Entity.hpp"

Entity::Entity(float X, float Y, float X_SPEED, float Y_SPEED, sf::Angle ANGLE, sf::Texture* TEXTURE)
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

const void Entity::draw(sf::RenderWindow& window) {
	sprite.setPosition(sf::Vector2f{ x, y });
	window.draw(sprite);
}

void Entity::update() {}

bool Entity::operator==(const sf::Texture* txt) const {
	return &sprite.getTexture() == txt;
}