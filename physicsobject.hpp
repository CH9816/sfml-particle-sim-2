#pragma once
#include "vector.hpp"
#include "globals.h"
#include "sfmlextras.h"






class PhysicsObject
{

	
	bool ready = false;

public:

	Vector pos, ppos, vel, acc, rForce;
	fpoint mass;

	sf::Vector2<fpoint> getPosSfml() {
		return sf::Vector2(pos.x, pos.y);
	}

	PhysicsObject() {
		this->pos = Vector();
		this->ppos = Vector();
		this->vel = Vector();
		this->acc = Vector();
		this->rForce = Vector();
		this->mass = 0;
	}

	PhysicsObject(Vector pos, fpoint mass = 1) {
		this->pos = pos;
		this->ppos = pos.copy();
		this->vel = Vector();
		this->acc = Vector();
		this->mass = mass;
		this->rForce = Vector();
		ready = true;
	}

	void accelerate(Vector delta) {
		this->acc += delta;
	}
	void applyForce(Vector force) {
		this->acc += force / this->mass;
	}






};









class Ball : public PhysicsObject, public sf::CircleShape
{
public:

	C colour;
	int cId;
	fpoint mass;

	Ball() = default;

	Ball(Vector pos, float radius, int colour, fpoint mass = 1)
		: PhysicsObject(pos), sf::CircleShape(radius, 10)
	{
		cId = colour;
		this->setFillColor(getColour(cId));
		this->colour = getColour(colour);
		//this->setFillColor(this->colour);
		//printColour(this->getFillColor());
		//print(cId);
		this->setRadius(radius);
		this->setOrigin(radius, radius);
		this->setPosition(pos.x, pos.y);
		this->mass = mass;
	}

	void update() {
		this->setPosition(this->getPosSfml());
		// TODO remove this shit
		this->setFillColor(this->colour);
	}

	int getColourIdThreadSafe() {
		// TODO
		return cId;
	}

};