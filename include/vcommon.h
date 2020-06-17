#pragma once
#include "vmacro.h"
#include "vcommonBase.h"

#define DEL(p) if (!p){ delete p; p = 0; }
#define DEL_A(p) if (!p){ delete[] p; p = 0; }

#define GET_SAFE_RET(var, ret, ...) (EXPAND(COMB(GET_SAFE_, EXPAND(NUM(__VA_ARGS__)))(var, ret, EXPAND(NUM(__VA_ARGS__)), __VA_ARGS__)))
#define GET_SAFE(var, ...) GET_SAFE_RET(var, 0, __VA_ARGS__)

#define EXIST(ret, ...) if(EXPAND(REPEAT_SEP(EXIST_F, OR_M, __VA_ARGS__))) return ret;
#define EXIST_V(...) EXIST(void, __VA_ARGS__)
#define EXIST_B(...) EXIST(false, __VA_ARGS__)
#define EXIST_I(...) EXIST(0, __VA_ARGS__)
