#pragma once
#include "vmacroBase.h"

#define EXPAND(...) __VA_ARGS__

#define TO_STRING(X) TO_STRING1(X)
#define COMB(X, Y) COMB1(X, Y)

//分隔符
#define COMMA_M(X) ,X
#define SEM_M(X) ;X
#define EXCP_M(X) !X
#define OR_M(X) ||X

//递增 递减 符号 相反数 
#define DEC(n) COMB(DEC_, n)//--
#define RIS(n) COMB(RIS_, n)//++
#define SIGN(n) COMB(SIGN_, n)//
#define OPP(n) COMB(OPP_, n)//-

//求参
#define NUM(...) EXPAND(_NUM_ARGS_CNT(0, ##__VA_ARGS__, _NUM_ARGS()))
#define GETARG(N, ...) EXPAND(COMB(GETARG_, N)(N, GETARG_F, __VA_ARGS__))
#define CUTARGS(N, ...) EXPAND(COMB(GETARG_, N)(N, CUTARGS_F, __VA_ARGS__))

//条件判断
#define SWITCH_CASE(conf, ...) EXPAND(GETARG(RIS(conf), ##__VA_ARGS__))
#define SWITCH_CASE_ARGS(conf, ...) EXPAND(COMB(GETARG_, RIS(conf))(RIS(conf), GETARG_F, GETARG, CUTARGS)(1, ##__VA_ARGS__))
#define IF(n) SWITCH_CASE(RIS(SIGN(n)), 1, 0, 1)

//算术运算
#define ADD(X, Y) COMB(ADD_, X)(X, Y)//+
#define SUB(X, Y) COMB(ADD_, X)(X, OPP(Y))//-

//关系运算
#define EQ(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 0, 1, 0)//==
#define NE(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 1, 0, 1)//!=
#define LS(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 1, 0, 0)//<
#define LE(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 1, 1, 0)//<=
#define GT(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 0, 0, 1)//>
#define GE(X, Y) SWITCH_CASE(RIS(SIGN(SUB(X, Y))), 0, 1, 1)//>=

//逻辑运算
#define NO(n) COMB(NO_, n)
#define OR(X, Y) NO(EQ(ADD(IF(X), IF(Y)), 0))
#define AD(X, Y) EQ(ADD(IF(X), IF(Y)), 2)

//最值
#define MIN(X, Y) SWITCH_CASE(LS(X, Y), Y, X)
#define MAX(X, Y) SWITCH_CASE(GT(X, Y), Y, X)

//REPEAT系列
#define REPEAT_VA(func, ...) EXPAND(COMB(REPEAT_VA_, EXPAND(NUM(##__VA_ARGS__)))(1, EXPAND(NUM(##__VA_ARGS__)), EXPAND(NUM(##__VA_ARGS__)), func, RIS, COMMA_M, __VA_ARGS__))
#define REPEAT_VA_SEP(func, sep, ...) EXPAND(COMB(REPEAT_VA_, EXPAND(NUM(##__VA_ARGS__)))(1, EXPAND(NUM(##__VA_ARGS__)), EXPAND(NUM(##__VA_ARGS__)), func, RIS, sep, __VA_ARGS__))
#define REPEAT(n, f, X) COMB(REPEAT_VA_, n)(1, 1, n, f, RIS, COMMA_M, X)
#define REPEAT_ZERO(n, f, X) COMB(f(0, X), COMMA_M(REPEAT(n, f, X)))
#define REVERSE_REPEAT(n, f, X) COMB(REPEAT_VA_, n)(n, 1, n, f, DEC, COMMA_M, X)
#define REVERSE_REPEAT_ZERO(n, f, X) COMB(REVERSE_REPEAT(n, f, X), COMMA_M(f(0, X)))