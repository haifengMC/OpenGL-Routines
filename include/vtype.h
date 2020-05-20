#pragma once

#include "vgl.h"
#include "vvalue.h"
#include "vmacro.h"

#define MUL_ARGS_F(n, T) T
#define MUL_ARGS_NF(n, T) T##n
#define MUL_ARGS_CONST_F(n, T) const T& t##n
#define MUL_ARGS(n, f, T) REPEAT(n, f, T)
#define MUL_CLASS(c, n) \
	template<typename T>\
	class c : public vValue<MUL_ARGS(n, MUL_ARGS_F, T)>\
	{\
	public:\
		c() : vValue<MUL_ARGS(n, MUL_ARGS_F, T)>() {}\
		c(const T& t) : vValue<MUL_ARGS(n, MUL_ARGS_F, T)>(MUL_ARGS(n, MUL_ARGS_F, t)) {}\
		c(MUL_ARGS(n, MUL_ARGS_CONST_F, T)) : vValue<MUL_ARGS(n, MUL_ARGS_F, T)>(MUL_ARGS(n, MUL_ARGS_NF, t)) {}\
		T& operator[](const unsigned int& index) { return *(T*)this->vValue<MUL_ARGS(n, MUL_ARGS_F, T)>::operator [](index); }\
		const T& operator[](const unsigned int& index) const { return *(const T*)this->vValue<MUL_ARGS(n, MUL_ARGS_F, T)>::operator [](index); }\
	};

MUL_CLASS(vDouble, 2)
MUL_CLASS(vTreble, 3)

typedef vDouble<GLfloat> vec2;
typedef vTreble<GLfloat> vec3;

