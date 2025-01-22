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
	//vectorList<thread> thList;

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

		init_threads();

	}

	~Simulation() {
		delete[] objects;
		for (bool* boolPtr : goList)
			delete boolPtr;
	}







	bool addBall(const Ball &obj) {
		if (objectCount + 1 >= MAX_OBJECT_COUNT)
			return false;
		//objects.push_back(obj);
		objects[objectCount] = obj;
		objectCount++;
		return true;
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
		Ball::maxId = 0;
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

	vectorList<thread> thList;
	vectorList<bool*> goList;
	abool run;


	void init_threads() {

		int simThreadCount = 4, index = 0;

		forcount(simThreadCount) {
			goList.emplace_back(new bool(false));
		}
		run = true;


		// building q tree thread
		// method object arg
		thList.emplace_back(thread(
			&Simulation::thread_buildQTree, this,
			ref(run), ref(*goList[0])
		));

		// doing collision detection
		thList.emplace_back(thread(
			&Simulation::thread_nlognParticleForce, this,
			ref(run), ref(*goList[1])
		));


	}

	void start_buildQTree_thread() {
		*goList[0] = true;
	}



	void simulate_threaded() {
		//start_buildQTree_thread();
		//nlogn_ParticleForce(stableTree);
		start_nlognParticleForce_thread();

	}
	
	void start_nlognParticleForce_thread() {
		*goList[1] = true;
	}


	void thread_nlognParticleForce(abool& run, bool& go) {
		while (run) {
			if (go) {
				go = false;

				Integrator::qTree tree = Integrator::qTree(
					-1, { 0, 0 }, SECONDARY_FORCE_END
				);

				float good = 0;
				forcount(objectCount)
					if (tree.addObj(&objects[i]))
						good++;

				print(good / objectCount);
					

				nlogn_ParticleForce(tree);

				renderEngine.tree = tree;
			}
			else {
				// todo
			}
		}


	}







	Integrator::qTree
		stableTree = Integrator::qTree(-1, { 0, 0 }, SECONDARY_FORCE_END),
		forwardTree = Integrator::qTree(-1, { 0, 0 }, SECONDARY_FORCE_END)
		;

	// keep complete locked version of the tree in stable tree
	//  make sure to update stable tree at least every x ms
	void thread_buildQTree(abool& run, bool& go) {

		//TpsClock tpsClock(50);
		//bool
		//	stableTreeLock = false,
		//	forwardTreeLock = false
		//	;

		//done = false;

		while (run) {

			if (go) {
				go = false;

				// make blank tree
				//Integrator::qTree tree =
				//	Integrator::qTree(-1, { 0,0 }, SECONDARY_FORCE_END)
				//	;

				forwardTree.clear();

				for (int i = 0; i < objectCount; i++)
					forwardTree.addObj(&objects[i]);

				stableTree.locked = true;
				stableTree = forwardTree;
				//stableTree.clear();
				//for (int i = 0; i < objectCount; i++)
				//	stableTree.addObj(&objects[i]);
				stableTree.locked = false;
				renderEngine.tree = forwardTree;
				//done = true;

			
				//print(stableTree.waitTicks);
			
			
			}


		}


	}




	Integrator::qTree tree = Integrator::qTree(-1, { 0, 0 }, SECONDARY_FORCE_END);
	void buildQtree() {

		renderEngine.tree = tree;

		tree.clear();

		for (int i = 0; i < objectCount; i++)
			tree.addObj(&objects[i]);

	}



	void nlogn_ParticleForce(Integrator::qTree& treeInp) {
		auto leafList = treeInp.getLeafList();
		
		for (int i = 0; i < objectCount; i++) {
			Ball& obj = objects[i];

			//vectorList<const vectorList<Ball*>*>
			//vectorList<Ball*>
			vectorList<int>
				BallIndexList = treeInp.getAllWithinRange(
					obj, leafList, objects, objectCount
				);

			//for (const vectorList<Ball*>* & listPtr : BallPtrListList) {
				//for (Ball* objPtr : *listPtr) {
				//	Ball& otherObj = *objPtr;

				//	// when adding objects too fast objPtr here points to incomplete object ?????


				//	if (objPtr != &objects[i])
				//		resolveInteraction(obj, otherObj);

				//}

			// TODO INSTEAD OF PASSING POINTERS TRY INDEXES

			//for (Ball*& otherBallPtr : BallPtrList) {
			//
			//	if (*otherBallPtr != objects[i]) {
			//		resolveInteraction(obj, *otherBallPtr);
			//	}

			//}

			for (int j : BallIndexList) {
				if (i != j) {
					resolveInteraction(objects[i], objects[j]);
				}
			}


		}
	}







	
	

	Timer timer, totalTimer;
	Vector oldMp;

	vectorList<int> ballIndexList;
	vectorList<Vector> dVectors;
	
	void simulate_single_threaded() {

		Vector mp = getMousePos(*window);

		totalTimer.start();

		//doCollisions();

		int i = 0;

		timer.start();
		buildQtree();
		timeList[i++] = "tree build time = " + string(timer.get());

		timer.start();
		//n2_particleForce();
		nlogn_ParticleForce(tree);
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