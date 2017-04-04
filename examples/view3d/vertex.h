#pragma once
#include "geometry.h"

struct vertex {
	Vec3f Position;
	Vec3f Normal;

	void flip() {
		Normal = -Normal;
	}

	/*
		interpolate(const vertex &other) const

		Create a new vertex between this vertex and `other` by linearly
		interpolating all properties using a parameter of `t`. Subclasses should
		override this to interpolate additional properties.
	*/
	vertex interpolate(const vertex &other, float t)
	{
		return vertex();
		//return Vertex.new(self.pos:lerp(other.pos, t), self.normal : lerp(other.normal, t))

	}
};