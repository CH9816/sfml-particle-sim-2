#pragma once
#include "defines.hpp"


struct Vector {
public:
	fpoint x, y;

	Vector(fpoint x, fpoint y) {
		this->x = x;
		this->y = y;
	}

	template <typename T> Vector(const sf::Vector2<T>& vec) {
		this->x = vec.x;
		this->y = vec.y;
	}

	Vector() {
		this->x = 0;
		this->y = 0;
	}

	Vector copy() {
		return Vector(x, y);
	}

	Vector operator* (fpoint scalar) const {
		return Vector(this->x * scalar, this->y * scalar);
	}
	Vector operator- (Vector vec) const {
		return Vector(this->x - vec.x, this->y - vec.y);
	}
	Vector operator+ (Vector vec) const {
		return Vector(this->x + vec.x, this->y + vec.y);
	}
	Vector operator/ (fpoint scalar) const {
		return Vector(this->x / scalar, this->y / scalar);
	}

	fpoint dot(Vector vec) const {
		return this->x * vec.x + this->y * vec.y;
	}
	fpoint magnitude() const {
		return sqrt(x * x + y * y);
	}
	fpoint mag() const {
		return this->magnitude();
	}




	void operator+= (Vector vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void operator-= (Vector vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void operator/= (fpoint scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void operator*= (fpoint scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

};