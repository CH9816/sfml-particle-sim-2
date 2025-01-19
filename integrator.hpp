#pragma once
#include "defines.hpp"
#include "integratorh.h"
#include "physicsobject.hpp"





class Integrator {
public:
	fpoint dt;
	IntegratorType iType;

	Integrator() {
		this->dt = 0;
		this->iType = IntegratorType::Euler;
	};

	Integrator(fpoint dt, IntegratorType type) {
		this->iType = type;
		this->dt = dt;
	}

	void integrate(Ball* objList, unsigned int objCount) {
		switch (iType) {
		case IntegratorType::Euler:
			integrateEuler(objList, objCount);
			break;
		case IntegratorType::Verlet:
			integrateVerlet(objList, objCount);
			break;
		}
	}

	void integrateEuler(Ball* objects, unsigned int objectCount) {
		for (int i = 0; i < objectCount; i++) {
			Ball& obj = objects[i];
			obj.vel += obj.acc * dt;
			obj.pos += obj.vel * dt;
			obj.acc = Vector();
		}
	}

	void integrateVerlet(Ball* objects, unsigned int objectCount) {
		for (int i = 0; i < objectCount; i++) {
			Ball& obj = objects[i];
			obj.ppos = obj.pos.copy();
			obj.pos += obj.vel * dt + (obj.acc * dt * dt * .5f);
			obj.vel += obj.acc * dt;

			obj.rForce = obj.acc.copy();
			obj.acc = Vector();
		}
	}


	struct qTree {

		Vector pos;
		int depth;
		vectorList<qTree> data;
		vectorList<Ball*> objPtrList;
		fpoint minSize;
		int maxObjCountBeforeSubdivide = 10;

		qTree(fpoint minSize) {
			depth = 0;
			data = {};
			pos = { 0, 0 };
			this->minSize = minSize;
		};

		qTree(int parentDepth, const Vector& pos, fpoint minSize) {
			this->depth = parentDepth + 1;
			this->pos = pos.copy();
			this->minSize = minSize;

			clear();
		}

		bool contains(const Ball& obj) {
			return
				obj.pos.x > pos.x &&
				obj.pos.x < pos.x + getSize() &&

				obj.pos.y > pos.y &&
				obj.pos.y < pos.y + getSize()
				;
		}

		// if it were to be subivided again would it be too small?
		bool isMinSize() const {
			return (getSize() / 2.f < minSize);
		}

		bool addObj(Ball* objPtr) {

			
			if (not this->contains(*objPtr))
				return false;


			if (not isLeaf()) {

			}

			//if (objPtrList.size() < maxObjCountBeforeSubdivide or isMinSize()) {
			//	objPtrList.emplace_back(objPtr);
			//	return true;
			//}
			//else {
			//	if (isLeaf())
			//		subDivide();

			//	for (qTree& tree : data) {
			//		if (tree.addObj(objPtr))
			//			return true;
			//	}
			//	
			//}

			//return false;
		}


		sf::Vector2f getPosVector(int i) const {

			const float halfEdgeLength = getSize() / 2.f;

			switch (i) {
			case 0:
				return vf(pos);
			case 1:
				return vf(pos + Vector(halfEdgeLength, 0));
			case 2:
				return vf(pos + Vector(0, halfEdgeLength));
			case 3:
				return vf(pos + Vector(halfEdgeLength, halfEdgeLength));
			};
		}

		fpoint getSize() const {
			return WORLD_SIZE / (this->depth == 0 ? 1 : pow(2, depth));
		}

		void clear() {
			this->data.clear();
			this->objPtrList.clear();
			this->data.reserve(4);
		}

		bool isLeaf() const {
			return data.size() == 0;
		}

		bool subDivide() {
			if (data.size() == 4)
				return false;


			forcount(4)
				data.emplace_back(
					qTree(
						depth,
						getPosVector(i),
						minSize
					)
				);

			for (Ball*& objPtr : objPtrList) {
				for (qTree& tree : data) {
					if (tree.addObj(objPtr))
						break;
				}
			}

			//clear();
			objPtrList.clear();

			return true;

		}


		void render(sf::RenderWindow& win) const {

			sf::RectangleShape rect(sf::Vector2f(getSize(), getSize()));
			rect.setPosition(vf(pos));
			rect.setFillColor(isMinSize() ? rgba(200, 0, 0, 100) : C::Transparent);
			rect.setOutlineColor(rgba(100, 100, 100, 100));
			rect.setOutlineThickness(5);
			win.draw(rect);

			sf::CircleShape temp(20);
			temp.setFillColor(isLeaf() ? C::White : C::Red);

			for (Ball* obj : objPtrList) {
				temp.setPosition(vf(obj->pos));
				win.draw(temp);
			}

			for (const qTree& tree : data)
				tree.render(win);



		}


	};


};