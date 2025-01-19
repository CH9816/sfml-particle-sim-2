#include "sfmlextras.h"
#include "SFML/Graphics.hpp"
#include "globals.h"
#include <iostream>
#include "defines.hpp"

float mouseScroll;


bool isPressed(sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

//template <typename T> void print(T t) {
//	std::cout << t << std::endl;
//}


bool handleEvents(sf::RenderWindow& win) {

	sf::Event event;

	mouseScroll = 0;

	while (win.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return false;

		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				mouseScroll = event.mouseWheelScroll.delta;
			}
		}

		if (isPressed(K::Escape)) {
			return false;
		}

		//print(mouseScroll);
	}

	return true;

}

void printColour(sf::Color c) {
	std::cout <<
		 "r: " << (int)c.r <<
		" g: " << (int)c.g <<
		" b: " << (int)c.b <<
		" a: " << (int)c.a <<
	std::endl;
}

bool isMousePressed(sf::Mouse::Button button) {
	return sf::Mouse::isButtonPressed(button);
}

C rgba(int r, int g, int b, int a) {
	return C(r, g, b, a);
}

inline sf::Color linearColour(float percent, const sf::Color& c1, const sf::Color& c2) {
	return sf::Color(
		c1.r + percent * (c2.r - c1.r),
		c1.g + percent * (c2.g - c1.g),
		c1.b + percent * (c2.b - c1.b)
	);
}

sf::Color getColourFromForce(float force, float maxForcePositive, float maxForceNegative) {

	//const float range = (maxForcePositive - maxForceNegative);
	//const float absForce = force - maxForceNegative;
	//return linearColour(absForce / range, rgb(255, 0, 0, 100), rgb(0, 0, 255, 100));

	sf::Color zeroColour = rgba(0, 0, 0, 0);

	if (force >= 0) {
		return linearColour(force / maxForcePositive, zeroColour, rgba(255, 0, 0, 100));
	}
	else {
		return linearColour(force / maxForceNegative, zeroColour, rgba(0, 0, 255, 100));
	}

}



