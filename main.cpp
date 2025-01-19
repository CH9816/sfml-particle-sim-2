#include "Includes.hpp"
#include "integratorh.h"
#include "simulator.hpp"
#include "defines.hpp"
#include "random.hpp"
#include "forcegetter.hpp"
#include "sfmlextras.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ scrw, scrh }), "SFML works!");

    ForceGetter fGetter = ForceGetter();
	Simulation sim = Simulation(window, IntegratorType::Verlet, &fGetter, Dt);
	RandomGen rand = RandomGen();

    forcount(0) {
        Ball ball = Ball(
            Vector(100 + rand.get() * 100, 100 * rand.get()),
            10,          // radius
            0          // colour
        );
        ball.setPosition(100, 100);
        ball.setFillColor(sf::Color::Green);

        sim.addBall(ball);

    }


    bool wasMousePressedLastLoop = false;


    while (window.isOpen())
    {

		if (not handleEvents(window))
			break;


        if (isMousePressed(Mouse::Button::Right)) {

            if (not wasMousePressedLastLoop or (isPressed(K::LControl))) {
                wasMousePressedLastLoop = true;

                auto mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                sim.addBall(Ball(
                    sf::Vector2f(mp) + sf::Vector2f(rand.get() * 10, rand.get() * 10),
                    10,
                    isPressed(K::Num1)? 1 : 0,
                    1
                ));
            }

        }
        else {
            wasMousePressedLastLoop = false;
        }

        
        sim.simulate();
        sim.draw();


    }

    window.close();
}