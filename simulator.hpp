#pragma once

#include "integratorh.h"
#include "integrator.hpp"
#include "render.hpp"
#include "sfmlextras.h"
#include "defines.hpp"





class Simulation {

	Integrator physicsEngine;
	sf::RenderWindow* window;
	Render renderEngine;
	vectorList<Ball> objects;
	ForceGetter fGetter;

public:

	void addBall(Ball obj) {
		objects.push_back(obj);
	}

	Simulation(sf::RenderWindow & window, IntegratorType integratorType, ForceGetter *fGetter, fpoint dt) {
		this->physicsEngine = Integrator(dt, integratorType);
		this->window = &window;
		this->renderEngine = Render(window, fGetter);
		this->fGetter = *fGetter;
	}

	void draw() {
		(*window).clear();
		renderEngine.render(objects);
		(*window).display();
	}

	void doCollisions_badly() {
		for (Ball& obj : objects) {
			for (Ball& obj2 : objects) {
				if (&obj != &obj2) {
					if (obj.getGlobalBounds().intersects(obj2.getGlobalBounds())) {
						Vector normal = obj.pos - obj2.pos;
						normal /= normal.magnitude();
						Vector relativeVelocity = obj.vel - obj2.vel;
						fpoint dot = relativeVelocity.dot(normal);
						if (dot < 0) {
							Vector impulse = normal * dot;
							obj.vel -= impulse;
							obj2.vel += impulse;
						}
					}
				}
			}
		}
	}



	void dampenMotion() {
		for (Ball& obj : objects) {
			obj.vel *= (1 - LINEAR_RESISTANCE * physicsEngine.dt);
		}
	}




	inline void resolveInteraction(Ball& p1, Ball& p2) {
		// working p1 -> p2
		// vector from p1 to p2
		const Vector norm = p2.pos - p1.pos;
		//if (norm.magsq() > fGetter.r1 * fGetter.r1) {
		//	return;
		//}



		const float normMag = norm.mag();
		const Vector normNormalised = norm / (normMag == 0 ? .001f : normMag);

		// impulse = force * time 
		p1.applyForce(
			normNormalised *
			fGetter.getForce(normMag, p1.getColourIdThreadSafe(), p2.getColourIdThreadSafe()) 
			//* dt
		);
		p2.applyForce(
			normNormalised *
			-fGetter.getForce(normMag, p2.getColourIdThreadSafe(), p1.getColourIdThreadSafe()) 
			//* -dt
		);

	}


	void n2_particleForce() {
		//for (Ball& obj : objects) {
		//	for (Ball& obj2 : objects) {
		//		if (&obj != &obj2) {
		//			resolveInteraction(obj, obj2);
		//		}
		//	}
		//}
		for (int i = 0; i < objects.size(); i++) {
			//for (int j : otherSector.pIndexList) {
			for (int j = i + 1; j < objects.size(); j++) {
				//if (i != j) {
				resolveInteraction(objects[i], objects[j]);
				//}

			}

		}
	}


	void keepWithinBoundary(Ball& obj) {
		if (obj.pos.y > scrh) {
			obj.applyForce(Vector(0, -BORDER_FORCE));
		}

		if (obj.pos.y < 0) {
			obj.applyForce(Vector(0, BORDER_FORCE));
		}

		if (obj.pos.x > scrw) {
			obj.applyForce(Vector(-BORDER_FORCE, 0));
		}

		if (obj.pos.x < 0) {
			obj.applyForce(Vector(BORDER_FORCE, 0));
		}
	}



	
	void simulate() {

		//doCollisions();
		n2_particleForce();
		dampenMotion();

		physicsEngine.integrate(objects);
		for (Ball& obj : objects) {

			//obj.accelerate(Vector(0, 9.8f));

			keepWithinBoundary(obj);


			if (isPressed(K::W)) {
				obj.accelerate(Vector(0, -20));
			}


			obj.update();
		}

	}

};