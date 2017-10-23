#include "stdafx.h"
#include "gGraphics.h"
#include <SFML\Graphics.hpp>



gGraphics::gGraphics(sf::RenderWindow &_window) : window(_window) {}
void gGraphics::Render() {
	sf::Event event;
	while (this->window.pollEvent(event))
	{
		Poll(event);
	}
	this->window.clear(sf::Color::Green);
	WindowRender();
	this->window.display();
}
