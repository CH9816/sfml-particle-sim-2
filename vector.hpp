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

	Vector copy() const {
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
	fpoint magsq() const {
		return x * x + y * y;
	}
	fpoint magnitude() const {
		return sqrt(magsq());
	}
	fpoint mag() const {
		return this->magnitude();
	}




	void operator+= (const Vector &vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void operator-= (const Vector &vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void operator/= (const fpoint &scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void operator*= (const fpoint &scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	Vector operator-() const {
		return Vector(-x, -y);
	}

};