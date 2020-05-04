#pragma once

#include "vgl.h"
#include "vvalue.h"


template<typename T>
class vDouble : public vValue<T, T>
{
public:
	vDouble() : vValue<T, T>() {}
	vDouble(const T& t) : vValue<T, T>(t, t) {}
	vDouble(const T& t1, const T& t2) : vValue<T, T>(t1, t2) {}
};

template<typename T>
class vTreble : public vValue<T, T, T>
{
public:
	vTreble() : vValue<T, T, T>() {}
	vTreble(const T& t) : vValue<T, T, T>(t, t) {}
	vTreble(const T& t1, const T& t2, const T& t3) : vValue<T, T, T>(t1, t2, t3) {}
};

typedef vDouble<GLfloat> vec2;
typedef vTreble<GLfloat> vec3;

