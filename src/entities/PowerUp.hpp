#pragma once

#include "Entity.hpp"
#include "Player.hpp"
#include <list>
#include <memory>

class PowerUp : public Entity {
  public:
	static void generate(std::list<std::unique_ptr<Entity>>* e, const std::unique_ptr<Player>& p);
	PowerUp(sf::Texture* TEXTURE);
};