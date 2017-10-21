#pragma once
#include <SFML\Graphics.hpp>
class gGraphics
{
public:
	gGraphics(sf::RenderWindow &window);
	void Render();
	sf::RenderWindow &window;
	void WindowRender();
	void Poll(sf::Event event);
};

