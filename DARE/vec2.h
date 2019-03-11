#pragma once
#include <iostream>

template<typename T>
class vec2
{
public:

	T x, y;

	vec2() : x(0), y(0) {}
	vec2(const T &xx) : x(xx), y(xx) {}
	vec2(const T &xx, const T &yy) : x(xx), y(yy) {}

	// OPERATOR

	vec2<T> operator  - () const { return vec2<T>(-x, -y); }

	vec2<T> operator + (const vec2<T> &v) const { return vec2<T>(x + v.x, y + v.y); }
	vec2<T> operator - (const vec2<T> &v) const { return vec2<T>(x - v.x, y - v.y); }
	vec2<T> operator * (const vec2<T> &v) const { return vec2<T>(x * v.x, y * v.y); }
	vec2<T> operator / (const vec2<T> &v) const { return vec2<T>(x / v.x, y / v.y); }

	vec2<T> operator + (const T &a) const { return vec2<T>(x + a, y + a); }
	vec2<T> operator - (const T &a) const { return vec2<T>(x - a, y - a); }
	vec2<T> operator * (const T &a) const { return vec2<T>(x * a, y * a); }
	vec2<T> operator / (const T &a) const { T inva = 1 / a;  return vec2<T>(x * inva, y * inva); }

	vec2<T>& operator += (const vec2<T> &v) { x += v.x; y += v.y;  return *this; }
	vec2<T>& operator -= (const vec2<T> &v) { x -= v.x; y -= v.y;  return *this; }
	vec2<T>& operator *= (const vec2<T> &v) { x *= v.x; y *= v.y;  return *this; }
	vec2<T>& operator /= (const vec2<T> &v) { x /= v.x; y /= v.y;  return *this; }

	vec2<T>& operator += (const T &a) { x += a; y += a; return *this; }
	vec2<T>& operator -= (const T &a) { x -= a; y -= a; return *this; }
	vec2<T>& operator *= (const T &a) { x *= a; y *= a; return *this; }
	vec2<T>& operator /= (const T &a) { T inva = 1 / a; x *= inva; y *= inva; return *this; }

	bool operator == (const vec2<T> &v) { return x == v.x && y == v.y; }
	bool operator != (const vec2<T> &v) { return x != v.x || y != v.y; }

	const T& operator [] (int i) const
	{
		return (&x)[i];
	}

	T& operator [] (int i)
	{
		return (&x)[i];
	}

	friend std::ostream& operator<<(std::ostream& os, const vec2<T> &v)
	{
		os << "<" << v.x << ", " << v.y << ">";
		return os;
	}

	// UTILITIES

	T length() const { return sqrt(x*x + y*y); }

	vec2& normalize()
	{
		T lengthSquare = x*x + y*y;

		if (length > 0)
		{
			*this *= 1 / lengthSquare;
		}

		return *this;
	}

	T dot(const vec2 &v) const { return x * v.x + y * v.y ; }
};


// UTILITIES OUTSIDE CLASS

template <typename T>
T length(const vec2<T> &v)
{
	return sqrt(v.x * v.x + v.y * v.y );
}

template <typename T>
void normalize(vec2<T> &v)
{
	T lengthSquare = v.x*v.x + v.y*v.y;

	if (length > 0)
	{
		v *= 1 / lengthSquare;
	}
}

template <typename T>
T dot(const vec2<T> &a, const vec2<T> &b)
{
	return sqrt(a.x * b.x + a.y * b.y);
}

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;
typedef vec2<double> vec2d;