#pragma once
#include "vcommonBase.h"

#define DEL(p) if (!p){ delete p; p = 0; }

#define GET_SAFE_RET(var, ret, ...) (EXPAND(COMB(GET_SAFE_, EXPAND(NUM(__VA_ARGS__)))(var, ret, EXPAND(NUM(__VA_ARGS__)), __VA_ARGS__)))
#define GET_SAFE(var, ...) GET_SAFE_RET(var, 0, __VA_ARGS__)

#define EXIST_F(n, X) (EXCP_M((X)))
#define EXIST(ret, ...) if(EXPAND(REPEAT_SEP(EXIST_F, OR_M, __VA_ARGS__))) return ret;
#define EXIST_V(...) EXIST(void, __VA_ARGS__)
#define EXIST_B(...) EXIST(false, __VA_ARGS__)
#define EXIST_I(...) EXIST(0, __VA_ARGS__)

#define ENUM_F(n, em, nm) em
#define ENUM_CASE_F(n, em, nm) case emName::em: os << nm;break
#define ENUM(name, ...) \
	enum class name { EXPAND(REPEAT_F(ENUM_F, 2, ##__VA_ARGS__)) };\
	std::ostream& operator<<(std::ostream& os, const name& emName)\
	{\
		switch (emName)\
		{\
			EXPAND(REPEAT_F_SEP(ENUM_CASE_F, 2, SEM_M, ##__VA_ARGS__));\
			default:os << TO_STRING(name)" ERR_TYPE";break;\
		}\
	}