#pragma once


#include "graphicc.h"
#include <math.h>

struct vec3 {
	union {
		struct { real x, y, z; };
		struct { real r, g, b; };
		struct { real s, t, p; };

		real v[3];
	};

	vec3() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	vec3(const vec3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	vec3(const real a, const real b, const real c) {
		x = a;
		y = b;
		z = c;
	}

	real lengthSquared() const {
		return x*x + y*y + z*z;
	}

	real norm() const {
		return sqrt(lengthSquared());
	}

	real dot(const vec3 &rhs) const 
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	vec3 cross(const vec3 &b) const
	{
		vec3 c;
		c.x = (y * b.z) - (z * b.y);
		c.y = (z * b.x) - (x * b.z);
		c.z = (x * b.y) - (y * b.x);
	
		return c;
	}

	vec3 & normalize() {
		real mag = 1/norm();
		*this *= mag;

		return *this;
	}

	// Overloading operators
	vec3 & operator=(const vec3& other) {
		if (this != &other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		return *this;
	}


	vec3 & operator *= (const real s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}

	vec3 & operator += (const vec3 & other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

		return *this;
	}

	vec3 & operator += (const real s) {
		this->x += s;
		this->y += s;
		this->z += s;

		return *this;
	}

	friend vec3 operator+(vec3 lhs, const vec3& rhs) {
		lhs += rhs;
		return lhs;
	}

	vec3 & operator -= (const vec3 & other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;

		return *this;
	}

	friend vec3 operator-(vec3 lhs, const vec3& rhs) {
		lhs -= rhs;
		return lhs;
	}

	// Indexing operators
	real & operator[](size_t idx) {
		switch (idx) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		}
	}

	const real & operator[](size_t idx) const {
		switch (idx) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		}
	}



};


