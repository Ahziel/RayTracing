#pragma once

#pragma once

#include <ostream>

template<typename T>
class Vec3
{
public:

	T x, y, z;

	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(const T &xx) : x(xx), y(xx), z(xx) {}
	Vec3(const T &xx, const T &yy, const T &zz) : x(xx), y(yy), z(zz) {}

	Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }

	Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }

	Vec3<T>& operator += (const Vec3<T> &v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vec3<T>& operator -= (const Vec3<T> &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	Vec3<T>& operator += (const T &a) { x += a; y += a; z += a; return *this; }
	Vec3<T>& operator -= (const T &a) { x += a; y += a; z += a; return *this; }
	Vec3<T>& operator *= (const T &a) { x += a; y += a; z += a; return *this; }
	Vec3<T>& operator /= (const T &a) { T inva = 1 / a;  x *= inva; y *= inva; z *= inva; return *this; }

	T operator[](int i) const
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
	}

	T& operator[](int i)
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
	}

	bool operator == (const Vec3<T> &v)
	{
		if (x == v.x && y == v.y && z == v.z) { return true; }
		return false;
	}

	bool operator != (const Vec3<T> &v)
	{
		if (x != v.x || y != v.y || z != v.z) { return true; }
		return false;
	}

	friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
	{
		os << v.x << " " << v.y << " " << v.z;
		return os;
	}

	// Compute the length of a vector
	T length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	T squareLength() const
	{
		return x * x + y * y + z * z;
	}

	// Normalize the vector
	Vec3<T>& normalize()
	{
		T length = this->length();
		if (length > 0) {
			T invLen = 1 / length;
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		return *this;
	}

	// Dot product
	T dot(const Vec3<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	// Cross product
	Vec3<T> cross(const Vec3<T>&v) const
	{
		return Vec3<T>(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
			);
	}

};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;

// Compute the length of a vector

template<typename T>
Vec3<T> operator + (const T &a, const Vec3<T> &v) { return Vec3<T>(v.x + a, v.y + a, v.z + a); }

template<typename T>
Vec3<T> operator - (const T &a, const Vec3<T> &v) { return Vec3<T>(v.x - a, v.y - a, v.z - a); }

template<typename T>
Vec3<T> operator * (const T &a, const Vec3<T> &v) { return Vec3<T>(v.x * a, v.y * a, v.z * a); }

template<typename T>
Vec3<T> operator + (const Vec3<T> &v, const T &a) { return Vec3<T>(v.x + a, v.y + a, v.z + a); }

template<typename T>
Vec3<T> operator - (const Vec3<T> &v, const T &a) { return Vec3<T>(a - v.x, a - v.y, a - v.z); }

template<typename T>
Vec3<T> operator * (const Vec3<T> &v, const T &a) { return Vec3<T>(v.x * a, v.y * a, v.z * a); }

template<typename T>
Vec3<T> operator / (const Vec3<T> &v, const T &a) { T inva = 1 / a; return Vec3<T>(v.x * inva, v.y * inva, v.z * inva); }

template<typename T>
T length(const Vec3<T> &v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Normalize the vector
template<typename T>
Vec3<T> normalize(Vec3<T> v)
{
	Vec3<T> res(v);

	T length = sqrt(res.x * res.x + res.y * res.y + res.z * res.z);

	if (length > 0)
	{
		T invLen = 1 / length;
		res.x *= invLen;
		res.y *= invLen;
		res.z *= invLen;
	}

	return res;
}

// Compute the dot product
template<typename T>
T dot(const Vec3<T> &a, const Vec3<T> &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Compute the cross product
template<typename T>
Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b)
{
	return Vec3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
		);
}