#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <list> 

class Entity{
	public:
	float x, y, dx, dy, radius, angle;
	bool life;
	sf::Sprite sprite;
	std::string name;
	
	Entity() {life=1;}
	
	Entity(float x, float y, float angle, float radius, sf::Sprite sprite){
		x=x; y=y; angle=angle; radius=radius, sprite=sprite;
	}

	virtual void update(){};

	void draw(sf::RenderWindow &app){
		sprite.setPosition(x, y);
		sprite.setRotation(angle + 90);
		app.draw(sprite);
	}
};
class Asteroid: public Entity{
	public:
	sf::Texture texture;
	Asteroid(){
		dx=rand()%8-4;
		dy=rand()%8-4;
		name="Asteroid";
	}
	void settings(float x, float y, float angle, float radius, sf::Texture texture){
    	x=x; y=y; angle=angle; radius=radius; texture=texture;
   	}
	void update(){
		x+=dx;
		y+=dy;
		if (x > 3440) x = 0;
		else if (x <= 0) x = 3440;
		if (y > 1440) y = 0;
		else if (y <= 0) y = 1440; //Do zrobienia
	}
};