#ifndef VEC2D_H
#define VEC2D_H

#include <math.h>

const float PI = 3.14159265359;


enum  Direction
{
	LEFT = 180, RIGHT = 0, UP = 270, DOWN = 90,
	UP_LEFT = 225, UP_RIGHT = 135, DOWN_RIGHT = 315, DOWN_LEFT = 45,
	NO_DIRECTION = -1
};


static float radian(float angle) { return 2 * PI * (angle / 360.f); };


template<class T> struct Vec2D { T x; T y; };

    //addition
template<class T, class A> Vec2D<T> operator+(Vec2D<T> vec1, Vec2D<A> vec2)
{
	return Vec2D<T>{ vec1.x + vec2.x, vec1.y + vec2.y };
}

template<class T> Vec2D<T> operator+(Vec2D<T> vec1, T number)
{
	return Vec2D<T>{ vec1.x + number, vec1.y + number};
}

template<class T> Vec2D<T> operator+(T number, Vec2D<T> vec1)
{
	return Vec2D<T>{ vec1.x + number, vec1.y + number};
}



	//subtracion
template<class T, class A> Vec2D<T> operator-(Vec2D<T> vec1, Vec2D<A> vec2)
{
	return Vec2D<T>{ vec1.x - vec2.x, vec1.y - vec2.y };
}

template<class T> Vec2D<T> operator-(Vec2D<T> vec1, T number)
{
	return Vec2D<T>{ vec1.x - number, vec1.y - number};
}

template<class T> Vec2D<T> operator-(T number, Vec2D<T> vec1)
{
	return Vec2D<T>{ number - vec1.x, number - vec1.y};
}



	//multiplication
template<class T> Vec2D<T> operator*(Vec2D<T> vec1, T number)
{
	return Vec2D<T>{ vec1.x* number, vec1.y * number };
}

template<class T> Vec2D<T> operator*(T number, Vec2D<T> vec1)
{
	return Vec2D<T>{ vec1.x* number, vec1.y * number };
}



	//division
template<class T> Vec2D<T> operator/(Vec2D<T> vec1, T number)
{
	return  Vec2D<T>{ vec1.x / number, vec1.y / number };
}

template<class T> Vec2D<T> operator/(T number, Vec2D<T> vec1)
{
	return Vec2D<T>{ number / vec1.x, number / vec1.y };
}



    //other operations
template<class T> Vec2D<T> normalize(Vec2D<T> vec)
{
	if (vec.x == 0 && vec.y == 0)
		return { 0,0 };

	T hip = sqrt(vec.x * vec.x + vec.y * vec.y);
	return { vec.x/hip, vec.y/hip};
}

template<class T, class A> T dotProduct(Vec2D<T> vec1, Vec2D<A> vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

template<class T> T getLength(Vec2D<T> vec)
{
	if (vec.x == 0 && vec.y == 0)
		return 0;
	else
		return sqrt(vec.x * vec.x + vec.y * vec.y);
}


#endif
