#pragma once

#include "Entity.hpp"
#include "Player.hpp"
#include <list>
#include <memory>

class PowerUp : public Entity {
  public:
	char type() override;
	static void generate(std::list<Entity*>* e, const std::unique_ptr<Player>& p);
	PowerUp(sf::Texture* TEXTURE);
};