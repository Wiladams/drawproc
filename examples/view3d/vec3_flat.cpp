#ifdef __cplusplus
extern "C" {
#endif

	inline void vec3_set(struct vec3 &a, const real x, const real y, const real z)
	{
		a.x = x;
		a.y = y;
		a.z = z;
	}

	inline struct vec3 vec3_clone(const struct vec3 &a)
	{
		struct vec3 c = { a.x, a.y, a.z };
		return c;
	}

	inline struct vec3 vec3_add(const struct vec3 a, const struct vec3 b)
	{
		struct vec3 c = { (a.x + b.x), (a.y + b.y), (a.z + b.z) };
		return c;
	}

	inline struct vec3 vec3_sub(const struct vec3 a, const struct vec3 b)
	{
		struct vec3 c = { (a.x - b.x) , (a.y - b.y) , (a.z - b.z) };
		return c;
	}

	inline struct vec3 vec3_mul(const struct vec3 a, const struct vec3 b)
	{
		struct vec3 c = { (a.x * b.x), (a.y * b.y), (a.z * b.z) };
		return c;
	}

	inline struct vec3 vec3_scale(const struct vec3 a, const real s)
	{
		struct vec3 c = { a.x*s, a.y*s, a.z*s };
		return c;
	}

	inline struct vec3 vec3_neg(const struct vec3 &a)
	{
		struct vec3 c = { -a.x, -a.y, -a.z };
		return c;
	}

	inline real vec3_dot(const struct vec3 a, const struct vec3 b)
	{
		return  a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline real vec3_lengthsq(const struct vec3 a)
	{
		return a.x*a.x + a.y*a.y + a.z*a.z;
	}

	inline real vec3_length(const struct vec3 a)
	{
		return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	}

	inline struct vec3 vec3_normalize(const struct vec3 a)
	{
		real s = 1.0 / vec3_length(a);
		struct vec3 c = { a.x*s, a.y*s, a.z*s };
		return c;
	}

	inline struct vec3 vec3_cross(const struct vec3 a, const struct vec3 b)
	{
		struct vec3 c = {
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x)
		};

		return c;
	}

	real vec3_distance(const vec3 a, const vec3 b)
	{
		return vec3_length(vec3_sub(b, a));
	}

	real vec3_radians_between_units(const struct vec3 a, const struct vec3 b)
	{
		return acos(vec3_dot(a, b));
	}

	real vec3_radians_between(const struct vec3 a, const struct vec3 b)
	{
		return vec3_radians_between_units(vec3_normalize(a), vec3_normalize(b));
	}

#ifdef __cplusplus
}
#endif
