#pragma once
#include <iostream>

template<typename T>
class vec4
{
public:

	T x, y, z, w;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(const T &xx) : x(xx), y(xx), z(xx), w(xx) {}
	vec4(const T &xx, const T &yy, const T &zz) : x(xx), y(yy), z(zz) {}

	// OPERATOR

	vec4<T> operator  - () const { return vec4<T>(-x, -y, -z); }

	vec4<T> operator + (const vec4<T> &v) const { return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
	vec4<T> operator - (const vec4<T> &v) const { return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
	vec4<T> operator * (const vec4<T> &v) const { return vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w); }
	vec4<T> operator / (const vec4<T> &v) const { return vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w); }

	vec4<T> operator + (const T &a) const { return vec4<T>(x + a, y + a, z + a, w + a); }
	vec4<T> operator - (const T &a) const { return vec4<T>(x - a, y - a, z - a, w - a); }
	vec4<T> operator * (const T &a) const { return vec4<T>(x * a, y * a, z * a, w * a); }
	vec4<T> operator / (const T &a) const { T inva = 1 / a;  return vec4<T>(x * inva, y * inva, z * inva, w * inva); }

	vec4<T>& operator += (const vec4<T> &v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	vec4<T>& operator -= (const vec4<T> &v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	vec4<T>& operator *= (const vec4<T> &v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	vec4<T>& operator /= (const vec4<T> &v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	vec4<T>& operator += (const T &a) { x += a; y += a; z += a; w += a; return *this; }
	vec4<T>& operator -= (const T &a) { x -= a; y -= a; z -= a; w -= a; return *this; }
	vec4<T>& operator *= (const T &a) { x *= a; y *= a; z *= a; w *= a; return *this; }
	vec4<T>& operator /= (const T &a) { T inva = 1 / a; x *= inva; y *= inva; z *= inva; w *= inva; return *this; }

	bool operator == (const vec4<T> &v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
	bool operator != (const vec4<T> &v) { return x != v.x || y != v.y || z != v.z || w != v.w; }

	const T& operator [] (int i) const
	{
		return (&x)[i];
	}

	T& operator [] (int i)
	{
		return (&x)[i];
	}

	friend std::ostream& operator<<(std::ostream& os, const vec4<T> &v)
	{
		os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
		return os;
	}

	// UTILITIES

	T length() const { return sqrt(x*x + y*y + z*z + w*w); }

	vec4& normalize()
	{
		T lengthSquare = x*x + y*y + z*z + w*w;

		if (length > 0)
		{
			*this *= 1 / lengthSquare;
		}

		return *this;
	}

	T dot(const vec4 &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

};


// UTILITIES OUTSIDE CLASS

template <typename T>
T length(const vec4<T> &v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template <typename T>
void normalize(vec4<T> &v)
{
	T lengthSquare = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;

	if (length > 0)
	{
		v *= 1 / lengthSquare;
	}
}

template <typename T>
T dot(const vec4<T> &a, const vec4<T> &b)
{
	return sqrt(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<double> vec4d;