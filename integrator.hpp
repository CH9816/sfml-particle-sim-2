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

	void integrate(vectorList<Ball>& objList) {
		switch (iType) {
		case IntegratorType::Euler:
			integrateEuler(objList);
			break;
		case IntegratorType::Verlet:
			integrateVerlet(objList);
			break;
		}
	}

	void integrateEuler(vectorList<Ball>&objList) {
		for (Ball& obj : objList) {
			obj.vel += obj.acc * dt;
			obj.pos += obj.vel * dt;
			obj.acc = Vector();
		}
	}

	void integrateVerlet(vectorList <Ball>& objList) {
		for (Ball& obj : objList) {
			obj.ppos = obj.pos.copy();
			obj.pos += obj.vel * dt + (obj.acc * dt * dt * .5f);
			obj.vel += obj.acc * dt;
			obj.acc = Vector();
		}
	}





};