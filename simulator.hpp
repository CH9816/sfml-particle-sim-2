#pragma once

#include "integratorh.h"
#include "integrator.hpp"
#include "render.hpp"
#include "sfmlextras.h"
#include "defines.hpp"
#include "timer.hpp"

#define forobjects() for (Ball *ball = objectCount > 0? objects[0] : nullptr ; ball != objects[objectCount] and ball != nullptr;)



class Simulation {

	Integrator physicsEngine;
	sf::RenderWindow* window;
	Render renderEngine;
	//vectorList<Ball> objects;
	Ball *objects;
	unsigned int objectCount = 0;
	ForceGetter *fGetterPtr;
	

public:

	vectorList<std::string> timeList;
	vectorList<thread> thList;

	int thcount = 4;



	Simulation(sf::RenderWindow & window, IntegratorType integratorType, ForceGetter* fGetterPtr, fpoint dt) {
		this->physicsEngine = Integrator(dt, integratorType);
		this->window = &window;
		this->renderEngine = Render(window, fGetterPtr);
		this->fGetterPtr = fGetterPtr;

		forcount(10)
			timeList.push_back("NULL");

		print("allocating object array");
		objects = new Ball[MAX_OBJECT_COUNT];
		print("done");

	}

	~Simulation() {
		delete[] objects;
	}







	void addBall(const Ball &obj) {
		//objects.push_back(obj);
		objects[objectCount++] = obj;
	}

	void draw() {
		(*window).clear();
		renderEngine.render(objects, objectCount);
		(*window).display();
	}




	void dampenMotion() {
		for (int i = 0; i < objectCount; i ++){
			Ball& obj = objects[i];

			fpoint velMagSq = obj.vel.magsq();
			fpoint mag = sqrt(velMagSq);
			const Vector axi = obj.vel / (mag == 0 ? .01f : mag) ;
			//obj.applyForce(-axi * velMagSq * QUADRATIC_RESISTANCE);
			obj.applyForce(-axi * mag * LINEAR_RESISTANCE);

			//obj.vel *= (1 - (LINEAR_RESISTANCE_OVER_DT) * physicsEngine.dt);
		}
	}

	void clearObjects() {
		objectCount = 0;
	}




	inline void resolveInteraction(Ball& p1, Ball& p2) {
		// working p1 -> p2
		// vector from p1 to p2
		const Vector norm = p2.pos - p1.pos;
		//if (norm.magsq() > fGetter.r1 * fGetter.r1) {
		//	return;
		//}



		const fpoint normMag = norm.mag();
		const Vector normNormalised = norm / (normMag == 0 ? .001f : normMag);

		p1.applyForce(
			normNormalised *
			fGetterPtr->getForce(normMag, p1.getColourIdThreadSafe(), p2.getColourIdThreadSafe()) 
			//* dt
		);
		p2.applyForce(
			normNormalised *
			-fGetterPtr->getForce(normMag, p2.getColourIdThreadSafe(), p1.getColourIdThreadSafe()) 
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
		for (int i = 0; i < objectCount; i++) {
			//for (int j : otherSector.pIndexList) {
			for (int j = i + 1; j < objectCount; j++) {
				//if (i != j) {
				resolveInteraction(objects[i], objects[j]);
				//}

			}

		}
	}


	void keepWithinBoundary(Ball& obj) {
		if (obj.pos.y > WORLD_SIZE) {
			obj.applyForce(Vector(0, -BORDER_FORCE));
		}

		if (obj.pos.y < 0) {
			obj.applyForce(Vector(0, BORDER_FORCE));
		}

		if (obj.pos.x > WORLD_SIZE) {
			obj.applyForce(Vector(-BORDER_FORCE, 0));
		}

		if (obj.pos.x < 0) {
			obj.applyForce(Vector(BORDER_FORCE, 0));
		}
	}


	



	Integrator::qTree tree = Integrator::qTree(-1, { 0, 0 }, SECONDARY_FORCE_END);
	void buildQtree() {

		renderEngine.tree = tree;

		tree.clear();

		for (int i = 0; i < objectCount; i++)
			tree.addObj(&objects[i]);






	}



	void nlogn_ParticleForce() {
		auto leafList = tree.getLeafList();
		
		for (int i = 0; i < objectCount; i++) {
			Ball& obj = objects[i];

			vectorList<vectorList<Ball*>*>
				BallPtrListList = tree.getAllWithinRange(obj, leafList);

			for (vectorList<Ball*>*& listPtr : BallPtrListList) {
				for (Ball* objPtr : *listPtr) {
					Ball& otherObj = *objPtr;

					if (objPtr != &objects[i])
						resolveInteraction(obj, otherObj);

				}
			}


		}
	}










	

	Timer timer, totalTimer;
	Vector oldMp;

	vectorList<int> ballIndexList;
	vectorList<Vector> dVectors;
	
	void simulate() {

		Vector mp = getMousePos(*window);

		totalTimer.start();

		//doCollisions();

		int i = 0;

		timer.start();
		buildQtree();
		timeList[i++] = "tree build time = " + string(timer.get());

		timer.start();
		//n2_particleForce();
		nlogn_ParticleForce();
		timeList[i++] = "particle force time = " + string(timer.get(-3)) + "ms";

		dampenMotion();

		timer.start();
		physicsEngine.integrate(objects, objectCount);
		timeList[i++] = "integration time = " + string(timer.get(-3)) + "ms";

		timer.start();
		//for (Ball& obj : objects) {
		for (int i = 0; i < objectCount; i++) {
			Ball& obj = objects[i];

			//obj.accelerate(Vector(0, 9.8f));

			keepWithinBoundary(obj);


			if (isPressed(K::W)) {
				obj.accelerate(Vector(0, -20));
			}


			obj.update();
		}



		
		if (isMousePressed(sf::Mouse::Right)) {
			int r = 50;
			Vector dMp = oldMp - mp;
			for (int i = 0; i < objectCount; i++) {
				Vector dPos = (objects[i].pos - mp);
				if (dPos.magsq() < r * r) {

					// if already in lists
					if (std::find(ballIndexList.begin(), ballIndexList.end(), i) != ballIndexList.end()) {
						continue;
					}

					dVectors.emplace_back(dPos);
					ballIndexList.emplace_back(i);
				}
			}
		}
		else {
			dVectors.clear();
			ballIndexList.clear();
		}

		for (int i = 0; i < dVectors.size(); i++) {
			Vector dPos = dVectors[i];
			int ballIndex = ballIndexList[i];
			objects[ballIndex].pos = mp + dPos;
		}

		

		timeList[i++] = "object update time = " + string(timer.get(-3)) + "ms";

		timeList[i++] =
			"total simulate time = " + string(totalTimer.get(-3)) +
			"ms, possible tps = " + string(1.f / totalTimer.get())
			;

		timeList[i++] =
			"n = " + string(objectCount);


		oldMp = mp.copy();
	}

};