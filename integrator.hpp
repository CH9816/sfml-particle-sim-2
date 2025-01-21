//#pragma once
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

		bool locked;

		qTree(fpoint minSize) {
			depth = 0;
			data = {};
			pos = { 0, 0 };
			this->minSize = minSize;
		};


		unsigned long waitTicks = 0;
		void waitForUnlock() {
			//return;
			while (locked) {
				// ehhhhh 
				waitTicks++;
			}
		}

		void clear(bool constructor = false) {
			if (not constructor)
				waitForUnlock();

			this->data.clear();
			this->objPtrList.clear();
			this->data.reserve(4);

			locked = false;
		}

		qTree(int parentDepth, const Vector& pos, fpoint minSize) {
			this->locked = true;
			this->depth = parentDepth + 1;
			this->pos = pos.copy();
			this->minSize = minSize;

			clear(true);

			this->locked = false;
		}

		fpoint getSize() const {
			return WORLD_SIZE / (this->depth == 0 ? 1 : pow(2, depth));
		}

		bool isLeaf() const {
			return data.size() == 0;
		}

		vectorList<int> getIndexList_ThreadSafe() {
			waitForUnlock();
			locked = true;	
			vectorList<int> output;
			if (isLeaf()) {
				for (Ball* ptr : objPtrList) {
					if (ptr != nullptr)
						output.emplace_back(ptr->id);
				}
			}

			locked = false;
			return output;

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

		bool subDivide() {
			if (data.size() == 4)
				return false;


			waitForUnlock();

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

		bool contains(const Ball& obj) const {
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

		bool isObjInTree(Ball& obj) const {
			
			return
				obj.pos.x > pos.x			  - SECONDARY_FORCE_END	and
				obj.pos.x < pos.x + getSize() + SECONDARY_FORCE_END and

				obj.pos.y > pos.y			  - SECONDARY_FORCE_END	and
				obj.pos.y < pos.y + getSize() + SECONDARY_FORCE_END
				;
		}


		//static vectorList<qTree*> leafList;

		void recurseCollectingLeaves(vectorList<qTree*>& leafList) {
			if (isLeaf())
				leafList.emplace_back(this);
			else {
				for (qTree& tree : data) {
					tree.recurseCollectingLeaves(leafList);
				}
			}
		}

		vectorList<qTree*> getLeafList() {
			vectorList<qTree*> list;
			recurseCollectingLeaves(list);
			return list;
		}

		//vectorList<const
		//	vectorList<Ball*>*
		//					 > getAllWithinRange(Ball& obj, vectorList<const qTree*>& leafList)
		//		const {

		//	vectorList<Ball*> list;


		//	vectorList<const vectorList<Ball*>*> list;
		//	//tickleafList();
		//	//print(leafList.size());
		//	for (const qTree*& treePtr : leafList) {
		//		const qTree& leaf = *treePtr;

		//		if (leaf.isObjInTree(obj)) {
		//			//auto temp = &leaf.objPtrList;

		//			// leaf pointer list no consistent
		//			// must copy pointers prob
		//			list.emplace_back(&leaf.objPtrList); // here
		//		}

		//	}

		//	return list;



		//}


		vectorList<int> getAllWithinRange(Ball& obj, vectorList<qTree*>& leafList,
										  Ball* objects, int objectCount)
				const {

			vectorList<int> list;


			//vectorList<const vectorList<Ball*>*> list;
			//tickleafList();
			//print(leafList.size());
			for (qTree*& treePtr : leafList) {
				qTree& leaf = *treePtr;

				if (leaf.isObjInTree(obj)) {
					//auto temp = &leaf.objPtrList;

					// leaf pointer list no consistent
					// must copy pointers prob
					//list.emplace_back(&leaf.objPtrList); // here
#
					// for all ball pointers in internal list, copy into list
					// no reference in loop to force copy, as objPtrList is 
					// changed by thread
					//for (Ball* obj : leaf.objPtrList) {
					//	list.push_back(obj);
					//}


					// not any more now were only returning index within objects array
					for (int index : leaf.getIndexList_ThreadSafe()) {
						list.push_back(index);
					}

				}

			}

			return list;



		}














		bool addObj(Ball* objPtr) {

			waitForUnlock();

			locked = true;

			if (not this->contains(*objPtr))
				locked = false;
				return false;

			if (this->isLeaf()) {

				if (isMinSize()) {
					objPtrList.emplace_back(objPtr);
					locked = false;
					return true;
				}
				// if can be dubdivided
				else {

					// if there is space
					if (objPtrList.size() < maxObjCountBeforeSubdivide) {
						objPtrList.emplace_back(objPtr);
						locked = false;
						return true;
					}

					// if there is not space and can be subdivided
					else {

						subDivide();

						for (qTree& tree : data) {
							if (tree.addObj(objPtr)) {
								locked = false;
								return true;
							}
						}
					}
				}


				if (objPtrList.size() < maxObjCountBeforeSubdivide or isMinSize()) {
					objPtrList.emplace_back(objPtr);
					locked = false;
					return true;



				}
			}
			// if not a leaf, add to children
			else {
				for (qTree& tree : data) {
					if (tree.addObj(objPtr)) {
						locked = false;
						return true;
					}
				}
			}

			locked = false;
			return false;

		}



		void renderExtra(sf::RenderWindow& win, Ball& ball0) {

			return;
			
			//vectorList<qTree*> leafList = getLeafList();

			//vectorList<const vectorList<Ball*>*>
			//auto
				//BallPtrListList = getAllWithinRange(ball0, leafList);

			int i = 0;
			sf::CircleShape temp(20);
			temp.setFillColor(rgba(128, 0, 128));

			////for (const vectorList<Ball*>* & listPtr : BallPtrListList) {
			//	for (Ball* & objPtr : *listPtr) {
			//		Ball& obj = *objPtr;
			//		temp.setPosition(obj.getPosSfml());
			//		win.draw(temp);


			//	}
			//}

		}



		void render(sf::RenderWindow& win) const {

			sf::RectangleShape rect(sf::Vector2f(getSize(), getSize()));
			rect.setPosition(vf(pos));
			rect.setFillColor(isMinSize() ? rgba(100, 0, 0, 50) : C::Transparent);
			rect.setOutlineColor(rgba(100, 100, 100, 100));
			rect.setOutlineThickness(5);
			win.draw(rect);

			sf::CircleShape temp(20);
			temp.setFillColor(isLeaf() ? C::White : C::Red);

			for (Ball* obj : objPtrList) {
				temp.setPosition(vf(obj->pos));
				//win.draw(temp);
			}

			



			for (const qTree& tree : data)
				tree.render(win);



		}


	};

};

//vectorList<Integrator::qTree*> Integrator::qTree::leafList = vectorList<qTree*>();