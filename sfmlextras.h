#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

bool isPressed(sf::Keyboard::Key key);

bool handleEvents(sf::RenderWindow& win);

template <typename T> void print(T t) {
	std::cout << t << std::endl;
}
void printColour(sf::Color c);

bool isMousePressed(sf::Mouse::Button button);

sf::Color rgba(int r, int g, int b, int a = 255);

inline sf::Color linearColour(float percent, const sf::Color& c1, const sf::Color& c2);

sf::Color getColourFromForce(float force, float maxForcePositive, float maxForceNegative);

sf::Color getColour(int id);