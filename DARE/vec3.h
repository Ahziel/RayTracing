#pragma once
#include <iostream>

template<typename T>
class vec3
{
public : 

	T x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(const T &xx) : x(xx), y(xx), z(xx) {}
	vec3(const T &xx, const T &yy, const T &zz) : x(xx), y(yy), z(zz) {}

	// OPERATOR

	vec3<T> operator  - () const { return vec3<T>(-x ,-y, -z); }

	vec3<T> operator + (const vec3<T> &v) const { return vec3<T>(x + v.x, y + v.y, z + v.z); }
	vec3<T> operator - (const vec3<T> &v) const { return vec3<T>(x - v.x, y - v.y, z - v.z); }
	vec3<T> operator * (const vec3<T> &v) const { return vec3<T>(x * v.x, y * v.y, z * v.z); }
	vec3<T> operator / (const vec3<T> &v) const { return vec3<T>(x / v.x, y / v.y, z / v.z); }

	vec3<T> operator + (const T &a) const { return vec3<T>(x + a, y + a, z + a); }
	vec3<T> operator - (const T &a) const { return vec3<T>(x - a, y - a, z - a); }
	vec3<T> operator * (const T &a) const { return vec3<T>(x * a, y * a, z * a); }
	vec3<T> operator / (const T &a) const { T inva = 1 / a;  return vec3<T>(x * inva, y * inva, z * inva); }

	vec3<T>& operator += (const vec3<T> &v) { x += v.x; y += v.y; z += v.z; return *this; }
	vec3<T>& operator -= (const vec3<T> &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	vec3<T>& operator *= (const vec3<T> &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	vec3<T>& operator /= (const vec3<T> &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	vec3<T>& operator += (const T &a) { x += a; y += a; z += a; return *this; }
	vec3<T>& operator -= (const T &a) { x -= a; y -= a; z -= a; return *this; }
	vec3<T>& operator *= (const T &a) { x *= a; y *= a; z *= a; return *this; }
	vec3<T>& operator /= (const T &a) { T inva = 1 / a; x *= inva; y *= inva; z *= inva; return *this; }

	bool operator == (const vec3<T> &v) { return x == v.x && y == v.y && z == v.z; }
	bool operator != (const vec3<T> &v) { return x != v.x || y != v.y || z != v.z; }

	const T& operator [] (int i) const
	{
		return (&x)[i];
	}

	T& operator [] (int i)
	{
		return (&x)[i];
	}

	friend std::ostream& operator<<(std::ostream& os, const vec3<T> &v)
	{
		os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
		return os;
	}

	// UTILITIES

	T length () const { return sqrt(x*x + y*y + z*z); }

	vec3& normalize() 
	{
		return *this / this->length();
 	}

	T dot(const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

	vec3<T> cross(const vec3 &v) const 
	{
		return vec3<T>(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}


};


// UTILITIES OUTSIDE CLASS

template <typename T>
T length(const vec3<T> &v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template <typename T>
vec3<T> normalize(vec3<T> v)
{
	T lengthSquare = v.x*v.x + v.y*v.y + v.z*v.z;

	if (lengthSquare > 0)
	{
		v *= 1/sqrt(lengthSquare);
	}

	return v;
}

template <typename T>
T dot(const vec3<T> &a, const vec3<T> &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
vec3<T> cross(const vec3<T> &a, const vec3<T> &b)
{
	return vec3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
		);
}


typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3<double> vec3d;