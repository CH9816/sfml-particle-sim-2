#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <utility>

bool isPressed(sf::Keyboard::Key key);

bool handleEvents(sf::RenderWindow& win);

template <typename T> void print(T t) {
	std::cout << t << std::endl;
}
void printColour(sf::Color c);

bool isMousePressed(sf::Mouse::Button button);

sf::Vector2f getMousePos(sf::RenderWindow& win);

sf::Color rgba(int r, int g, int b, int a = 255);

inline sf::Color linearColour(float percent, const sf::Color& c1, const sf::Color& c2);

sf::Color getColourFromForce(float force, float maxForcePositive, float maxForceNegative);

sf::Color getColour(int id);

int isInKeyListenerList(sf::Keyboard::Key key);

bool isPressedOnce(sf::Keyboard::Key key);

std::vector<std::pair<int, int>> divideInclusive(int start, int end, int n, bool overlap = true);

template<typename T>
sf::Vector2f vf(const T& vec) {
	return sf::Vector2f(vec.x, vec.y);
}


