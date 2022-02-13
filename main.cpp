#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

float degToRad(float deg){
	return deg * M_PI / 180;
}

int main()
{
	// create fullscreen window
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	static int Width=desktopMode.width, Height=desktopMode.height;
	sf::RenderWindow window(sf::VideoMode(Width, Height, desktopMode.bitsPerPixel), "Asteroids - Macieson", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60); 




	// load textures
	sf::Texture tPlayer;
	tPlayer.loadFromFile("player.png");
	tPlayer.setSmooth(true);

	// set player sprite
	sf::Sprite player;
	player.setTexture(tPlayer);
	player.setOrigin(tPlayer.getSize().x / 2, tPlayer.getSize().y / 2);
	//player.setOrigin(player.getGlobalBounds().width / 2, player.getGlobalBounds().height / 2);
	player.setPosition(window.getView().getCenter());

	//bool upFlag = false, downFlag = false, leftFlag = false, rightFlag = false;
    sf::Clock timer;

	int x = player.getPosition().x, y = player.getPosition().y;
	float dx=0, dy=0, angle=0;
	bool thrust;
	
	//float maxSpeed = 15;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
            if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape:	window.close(); break;
					
					case sf::Keyboard::Up:		thrust = true; break;
					case sf::Keyboard::Left:	angle-=3; break;
					case sf::Keyboard::Right:	angle+=3; break;
					default: 					thrust = false; break;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
				case sf::Keyboard::Up:		thrust = false; break;
				}
			}
		}

/* 
        if (leftFlag) player.setRotation(player.getRotation()-2);
		if (rightFlag) player.setRotation(player.getRotation()+2);
        //if (upFlag) player.setPosition(player.getPosition().x, player.getPosition().y-playerSpeed);
		if (upFlag) thrust=true;
        // if (downFlag) player.setPosition(player.getPosition().x, player.getPosition().y+playerSpeed);
*/ 

		if(thrust){
			dx+=cos(degToRad(angle))*0.2;
			dy+=sin(degToRad(angle))*0.2;
		}else{
			dx*=0.99;
			dy*=0.99;
		}

		float maxSpeed=10;
		float speed = sqrt(dx*dx+dy*dy);
		if (speed>maxSpeed){
			dx *= maxSpeed/speed;
			dy *= maxSpeed/speed;
		}

		x+=dx;
		y+=dy;

		std::cout << x << " " << y << "\n";
		//std::cout << desktopMode.width << " " << desktopMode.height << "\n";


		if(x>Width) x=0;
		else if(x<=0) x=Width;		
		if(y>Height) y=0;
		else if(y<=0) y=Height;
		
		//if(x>desktopMode.width) x=0; if (x<=0) x=desktopMode.width;
		//if(y>desktopMode.height) y=0; if (y<=0) y=desktopMode.height;	



		player.setPosition(x,y);
		player.setRotation(angle+90);


	window.clear();
	window.draw(player);
	window.display();

	}
return 0;
}