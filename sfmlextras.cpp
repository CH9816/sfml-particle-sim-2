#include "sfmlextras.h"
#include "SFML/Graphics.hpp"
#include "globals.h"
#include <iostream>
#include <utility>
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
			win.close();
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

sf::Vector2f getMousePos(sf::RenderWindow& win) {
	return win.mapPixelToCoords( (sf::Mouse::getPosition(win)) );
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


struct KeyListener {
	K::Key key;
	bool wasPressedLastUpdate;
	KeyListener(K::Key key) {
		this->key = key;
		wasPressedLastUpdate = false;
	}
};

static vectorList<KeyListener> keyListenerList;

static int isInKeyListenerList(K::Key key) {
	int i = 0;
	for (auto& keyL : keyListenerList) {
		if (keyL.key == key)
			return i;
		i++;
	}
	return -1;
}

bool isPressedOnce(K::Key key) {

	int index = isInKeyListenerList(key);

	if (not isPressed(key)) {
		if (index == -1)
			keyListenerList.push_back(KeyListener(key));
		else
			keyListenerList[index].wasPressedLastUpdate = false;
		return false;
	}



	if (index != -1) {
		KeyListener& listener = keyListenerList[index];
		if (listener.wasPressedLastUpdate) {
			return false;
		}
		else {
			listener.wasPressedLastUpdate = true;
			return true;
		}
	}
}

vectorList<std::pair<int, int>> divideInclusive(int start, int end, int n, bool overlap) {
	vectorList<std::pair<int, int>> list;

	if (start == 0 and end == 0) {
		forcount(n) {
			list.push_back(std::pair<int, int>(0, 0));
		}
		return list;
	}

	int per = overlap 
		? (end - start) / (n - 1) 
		: (end - start - n + 1) / (n - 1);

	while (start + per < end) {
		list.push_back(std::pair(start, start + per));
		start += per;
		if (not overlap)
			start += 1;	
	}

	if (list.size() < n) {
		list.push_back(std::pair(start, end));
	}
	else {
		list[list.size() - 1].second = end;
	}
	

	return list;
}



