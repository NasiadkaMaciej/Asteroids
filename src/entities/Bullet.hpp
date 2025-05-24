#pragma once

#include "Entity.hpp"
#include "utils.hpp"

class Bullet : public Entity {
  public:
	Bullet(float X, float Y, sf::Angle ANGLE, sf::Texture* TEXTURE, Scale s);
	void update() override;
};