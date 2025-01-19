#pragma once

#include "globals.h"
#include "physicsobject.hpp"
#include "defines.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"


#include "sfmlextras.h"
#include "forcegetter.hpp"
#include <String>


//template void print<std::string>(std::string t);


vectorList<C> colourIdMap = { rgba(255, 0, 0), rgba(0, 255, 0), rgba(0, 0, 255)};
C getColour(int id) {
	return colourIdMap[id];
}







class Render
{

	sf::RenderWindow* windowPtr;
	ForceGetter* forceGetter;

	void init() {
		oldMp = sf::Mouse::getPosition(*windowPtr);
	}

	sf::RenderWindow& getWindow() const {
		if (windowPtr != nullptr)
			return *windowPtr;
		else {
			print("windowPtr is nullptr");
			exit(2);
		}
	}

public:

	Render() {
		this->windowPtr = nullptr;
	};

	Render(sf::RenderWindow &win, ForceGetter *forceGetter) {
		this->windowPtr = &win;
		this->forceGetter = forceGetter;
		init();
	}

	void drawLine(int x1, int y1, int x2, int y2, C colour) {
		sf::VertexArray lines(sf::Lines, 2);
		lines.append(sf::Vertex(sf::Vector2f(x1, y1), colour));
		lines.append(sf::Vertex(sf::Vector2f(x2, y2), colour));
		(*windowPtr).draw(lines);
	}



	// https://github.com/SFML/SFML/wiki/Source:-Zoom-View-At-%28specified-pixel%29
	void zoomViewAtMp(float zoom)
	{

		sf::RenderWindow& window = *windowPtr;
		const sf::Vector2i mp = sf::Mouse::getPosition(window);
		const sf::Vector2f beforeCoord{ window.mapPixelToCoords(mp) };

		sf::View view{ window.getView() };
		view.zoom(zoom);
		window.setView(view);
		const sf::Vector2f 
			afterCoord{ window.mapPixelToCoords(mp) },
			offsetCoords{ beforeCoord - afterCoord };
		view.move(offsetCoords);
		window.setView(view);
	}

	void panView(const sf::Vector2i& pan) {
		sf::RenderWindow& window = *windowPtr;
		sf::View view = window.getView();
		view.move(sf::Vector2f(pan) * view.getSize().x / (float)scrw);
		window.setView(view);
	}

	sf::Vector2i oldMp;

	void handleZoomAndPan() {

		sf::Vector2i mp = sf::Mouse::getPosition(*windowPtr);
		sf::Vector2i dMp = oldMp - mp;

		if (isMousePressed(Mouse::Left)) 
			panView(dMp);

		if (mouseScroll != 0)
			zoomViewAtMp(1 + -mouseScroll * .1f);

		//print(mouseScroll);

		oldMp = mp;





	}


	


	void renderMatrix() {

		auto& win = getWindow();


		auto oldView = win.getView();
		win.setView(win.getDefaultView());






		// draw matrix
		float guiLeftWidth = scrw / 10.F;
		float matrixSize = guiLeftWidth - 20.f;
		float matrixOffset = 10;
		float smallRectSize = matrixSize / colourIdMap.size();
		auto matrixPos = sf::Vector2f(matrixOffset, matrixOffset + smallRectSize);
		sf::RectangleShape rect(sf::Vector2f(matrixSize, matrixSize));
		rect.setFillColor(C::Transparent);
		rect.setOutlineColor(rgba(100, 100, 100, 100));
		rect.setOutlineThickness(1);
		int matrixOpacity = 100; // out of 255

		rect.setSize(sf::Vector2f(smallRectSize, smallRectSize));


		float
			x = matrixPos.x,
			y = matrixPos.y
			;
		int i = 0;

		// get max val
		float maxP = 0, maxN = 100;
		for (auto& row : forceGetter->forceMaxMatrix) {
			for (auto val : row) {
				if (val > maxP) {
					maxP = val;
				}
				if (val < maxN) {
					maxN = val;
				}
			}
		}

		for (auto& row : forceGetter->forceMaxMatrix) {
			rect.setPosition(x, y);
			auto c = colourIdMap[i++];
			rect.setFillColor(c);

			x += smallRectSize;


			for (auto val : row) {
				rect.setPosition(x, y);
				x += smallRectSize;
				rect.setFillColor(getColourFromForce(val, maxP, maxN));
				auto c = rect.getFillColor(); c.a *= (matrixOpacity / 255.f); rect.setFillColor(c);
				win.draw(rect);
			}

			x = matrixOffset;
			y += smallRectSize;

		}

		i = 0;
		sf::CircleShape left(rect.getSize().x / 4.f);
		sf::CircleShape right(rect.getSize().x / 4.f);

		left.setOrigin(-left.getRadius(), -left.getRadius());
		right.setOrigin(-right.getRadius(), -right.getRadius());

		for (auto c : colourIdMap) {



			
			left.setPosition(
				matrixPos.x,
				matrixPos.y +
					(float)i / colourIdMap.size() * matrixSize
			);

			right.setPosition(
				matrixPos.x + smallRectSize +
					(float)i / colourIdMap.size() * matrixSize,
				matrixPos.y - smallRectSize
			);

			c.a *= matrixOpacity / 255.f;

			left.setFillColor(c);
			right.setFillColor(c);

			win.draw(left); win.draw(right);
				

			






			i++;

		}





		//for (auto& row : forceGetter->forceMaxMatrix) {

		//	rect.setPosition(x, y);
		//	rect.setFillColor(colourIdMap[i++]);
		//	auto c = rect.getFillColor(); c.a *= (matrixOpacity / 255.f); rect.setFillColor(c);
		//	win.draw(rect);

		//	rect.setFillColor(C::Transparent);

		//	x += smallRectSize;

		//	for (auto val : row) {
		//		rect.setPosition(x, y);

		//		x += smallRectSize;

		//		rect.setFillColor(getColourFromForce(val, maxP, maxN));
		//		auto c = rect.getFillColor(); c.a *= (matrixOpacity / 255.f); rect.setFillColor(c);
		//		win.draw(rect);




		//	}
		//	y += smallRectSize;
		//	x = matrixOffset;
		//}

		//i = 0;
		//x = matrixPos.x + smallRectSize;
		//y = matrixPos.y - smallRectSize;



		//for (sf::Color c : colourIdMap) {
		//	//rect.setPosition(x, y);
		//	//x += smallRectSize;
		//	//rect.setFillColor(colourIdMap[i++]);
		//	////auto c = rect.getFillColor(); 
		//	////c.a *= (matrixOpacity / 255.f);
		//	//rect.setFillColor(c);
		//	printColour(c);
		//	win.draw(rect);
		//}

		////print(colourIdMap[0].b);
		////print(colourIdMap.size());

		win.setView(oldView);



	}

	Integrator::qTree tree = Integrator::qTree(0);

	void drawQtree() {
		tree.render(*windowPtr);
	}



	void render(Ball* objList, unsigned int objCount) {
		sf::RenderWindow& win = getWindow();




		C attrC = rgba(100, 0, 0, 100);
		C repC = rgba(0, 0, 100, 100);

		sf::CircleShape forceFieldRep(REPULSION_START / 2.f);
		sf::CircleShape forceFieldAttr(SECONDARY_FORCE_END / 2.f);

		forceFieldAttr.setFillColor(attrC);
		forceFieldRep.setFillColor(repC);

		forceFieldAttr.setOrigin(forceFieldAttr.getRadius(), forceFieldAttr.getRadius());	
		forceFieldRep.setOrigin(forceFieldRep.getRadius(), forceFieldRep.getRadius());
		

		sf::VertexArray forceLine(sf::Lines, 2);
		forceLine.append(sf::Vertex()); forceLine.append(sf::Vertex());


		//for (Ball& obj : objects) {
		for (int i = 0; i < objCount; i++){

			Ball& obj = objList[i];

			win.draw(obj);

			if (isPressed(K::Tab))
			{
				forceFieldAttr.setPosition(obj.getPosition());
				forceFieldRep.setPosition(obj.getPosition());
				win.draw(forceFieldAttr); win.draw(forceFieldRep);	

				forceLine[0].position = obj.getPosSfml();
				forceLine[1] = vf(obj.pos + obj.rForce);

				win.draw(forceLine);

			}



		}

		handleZoomAndPan();


		// border
		sf::RectangleShape border;
		border.setFillColor(sf::Color::Transparent);
		border.setOutlineColor(sf::Color::White);
		border.setOutlineThickness(5);
		border.setSize(sf::Vector2f(WORLD_SIZE, WORLD_SIZE));
		border.setPosition(0, 0);
		win.draw(border);

		//
		renderMatrix();


		// ball at mouse
		sf::CircleShape mpCircle(3);
		mpCircle.setOrigin(mpCircle.getRadius(), mpCircle.getRadius());	
		mpCircle.setPosition(win.mapPixelToCoords(sf::Mouse::getPosition(win)));
		//win.draw(mpCircle);

		drawQtree();


	}
};