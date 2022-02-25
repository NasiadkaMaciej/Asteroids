#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

int
main()
{
  // create fullscreen window
  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  static int Width = desktopMode.width, Height = desktopMode.height;
  sf::RenderWindow window(
	sf::VideoMode(Width, Height, desktopMode.bitsPerPixel),
	"Asteroids - Macieson",
	sf::Style::Fullscreen);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  // load textures
  sf::Texture tPlayer, tAsteroid;
  tPlayer.loadFromFile("player.png");
  tAsteroid.loadFromFile("asteroidBig.png");
  tPlayer.setSmooth(true);
  tAsteroid.setSmooth(true);

  // set player sprite
  sf::Sprite player;
  player.setTexture(tPlayer);
  player.setOrigin(tPlayer.getSize().x / 2, tPlayer.getSize().y / 2);
  player.setOrigin(player.getGlobalBounds().width / 2,
				   player.getGlobalBounds().height / 2);
  player.setPosition(window.getView().getCenter());

  /*
  std::list<Entity*> entities;

  for(int i=0; i<15; i++){
				  Asteroid *a = new Asteroid();
				  a->settings(rand()%Width, rand()%Height, rand()%360, 25,
  tAsteroid); entities.push_back(a);
  }
  */

  // float x,y;
  float x = player.getPosition().x, y = player.getPosition().y;
  float dx = 0, dy = 0, angle = 0;
  bool thrust;

  float degToRad = M_PI / 180;

  // sf::Clock deltaClock;

  while (window.isOpen()) {
	// sf::Time deltaTime = deltaClock.restart();
	sf::Event event;
	while (window.pollEvent(event)) {
	  if (event.type == sf::Event::Closed)
		window.close();

	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		angle += 3;
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		angle -= 3;
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		thrust = true;
	  else
		thrust = false;
	}

	if (thrust) {
	  dx += cos(angle * degToRad) * 0.2;
	  dy += sin(angle * degToRad) * 0.2;
	} else {
	  dx *= 0.99;
	  dy *= 0.99;
	}

	int maxSpeed = 15;
	float speed = sqrt(dx * dx + dy * dy);
	if (speed > maxSpeed) {
	  dx *= maxSpeed / speed;
	  dy *= maxSpeed / speed;
	}

	x += dx;
	y += dy;

	if (x >= desktopMode.width)
	  x = 0;
	else if (x <= 0)
	  x = desktopMode.width;
	if (y >= desktopMode.height)
	  y = 0;
	else if (y <= 0)
	  y = desktopMode.height;

	/*if (thrust){
					dx = 10 * cos(degToRad(player.getRotation()-90));
					dy = 10 * sin(degToRad(player.getRotation()-90));
	}else{
					dx *= 0.99;
					dy *= 0.99;
	}

	float maxSpeed = 10;
	float speed = sqrt(dx * dx + dy * dy);
	if (speed > maxSpeed){
					dx *= maxSpeed / speed;
					dy *= maxSpeed / speed;
	}

	x += dx;
	y += dy;

	//std::cout << x << " " << y << "\n";
	// std::cout << desktopMode.width << " " << desktopMode.height << "\n";

	if (x > Width) x = 0;
	else if (x <= 0) x = Width;
	if (y > Height) y = 0;
	else if (y <= 0) y = Height;
	*/

	// if(x>desktopMode.width) x=0; if (x<=0) x=desktopMode.width;
	// if(y>desktopMode.height) y=0; if (y<=0) y=desktopMode.height;

	player.setPosition(x, y);
	player.setRotation(angle + 90);
	/*
					for(auto i=entities.begin();i!=entities.end();){
									Entity *e = *i;
									e->update();
									if (e->life==false) {i=entities.erase(i);
	   delete e;} else i++;
					}
					*/

	sf::Font font;
	font.loadFromFile("UbuntuMono-B.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString(std::to_string(x) + ":" + std::to_string(y) + "\n" +
				   std::to_string(desktopMode.width) + ":" +
				   std::to_string(desktopMode.height));
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::White);
	text.setPosition(100, 100);

	window.clear();
	window.draw(player);
	window.draw(text);
	// for(auto i:entities) i->draw(window);
	window.display();
  }
  return 0;
}