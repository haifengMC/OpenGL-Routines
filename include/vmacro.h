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
#define SPACE_M(X) X

//递增 递减 符号 相反数 取模
#define DEC(n) COMB(DEC_, n)//--
#define RIS(n) COMB(RIS_, n)//++
#define SIGN(n) COMB(SIGN_, n)//
#define OPP(n) COMB(OPP_, n)//-
#define ABS(n) SWITCH_CASE(EQ(SIGN(n), _1), n, OPP(n))//||

//求参
#define NUM(...) EXPAND(_NUM_ARGS_CNT(0, ##__VA_ARGS__, _NUM_ARGS()))
#define GETARG(N, ...) EXPAND(COMB(GETARG_, N)(N, GETARG_F, __VA_ARGS__))
#define CUTARGS(N, ...) EXPAND(COMB(GETARG_, N)(N, CUTARGS_F, __VA_ARGS__))
#define CUTARGS_B(N, ...) EXPAND(COMB(CUTARGS_B_, N)(N, __VA_ARGS__))

//条件判断
#define SWITCH_CASE(conf, ...) EXPAND(GETARG(RIS(conf), ##__VA_ARGS__))
#define SWITCH_CASE_ARGS_N(conf, n, ...) EXPAND(COMB(GETARG_, RIS(conf))(RIS(conf), GETARG_F, CUTARGS_B, CUTARGS)(n, ##__VA_ARGS__))
#define SWITCH_CASE_ARGS(conf, ...) EXPAND(SWITCH_CASE_ARGS_N(conf, 1, ##__VA_ARGS__))
#define IF(n) SWITCH_CASE(RIS(SIGN(n)), 1, 0, 1)

//算术运算
#define ADD(X, Y) COMB(ADD_, X)(X, Y)// +
#define SUB(X, Y) COMB(ADD_, X)(X, OPP(Y))// -
#define MUL(X, Y) COMB(MUL_, X)(X, Y, 0)// *
#define DIV(X, Y) MUL(COMB(DIV_, ABS(X))(ABS(X), ABS(Y), 0), MUL(SIGN(X), SIGN(Y)))// /

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
#define REPEAT_N_F_SEP(n, f, fn, sep, ...) COMB(REPEAT_, n)(1, DIV(EXPAND(NUM(##__VA_ARGS__)), fn), n, f, fn, RIS, sep, ##__VA_ARGS__)
#define REPEAT_N_F_SEP_ZERO(n, f, fn, sep, ...) COMB(f(0, CUTARGS_B(fn, ##__VA_ARGS__)), COMMA_M(REPEAT_N_F_SEP(n, f, fn, sep, CUTARGS(1, ##__VA_ARGS__))))
#define REVERSE_REPEAT_N_F_SEP(n, f, fn, sep, ...) COMB(REPEAT_, n)(n, DIV(EXPAND(NUM(##__VA_ARGS__)), fn), n, f, fn, DEC, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_SEP_ZERO(n, f, fn, sep, ...) COMB(REVERSE_REPEAT_N_F_SEP(n, f, fn, sep, ##__VA_ARGS__), COMMA_M(f(0, SWITCH_CASE(LS(n, EXPAND(NUM(##__VA_ARGS__))), GETARG(EXPAND(NUM(##__VA_ARGS__)), ##__VA_ARGS__), GETARG(n, ##__VA_ARGS__)))))

#define REPEAT_N_F(n, f, fn, ...) EXPAND(REPEAT_N_F_SEP(n, f, fn, COMMA_M, ##__VA_ARGS__))
#define REPEAT_N_F_ZERO(n, f, fn, ...) EXPAND(REPEAT_N_F_SEP_ZERO(n, f, fn, COMMA_M, ##__VA_ARGS__))
#define REVERSE_REPEAT_N_F(n, f, fn, ...) EXPAND(REVERSE_REPEAT_N_F_SEP(n, f, fn, COMMA_M, ##__VA_ARGS__))
#define REVERSE_REPEAT_N_F_ZERO(n, f, fn, ...) EXPAND(REVERSE_REPEAT_N_F_SEP_ZERO(n, f, fn, COMMA_M, ##__VA_ARGS__))

#define REPEAT_F_SEP(f, fn, sep, ...) EXPAND(REPEAT_N_F_SEP(DIV(EXPAND(NUM(##__VA_ARGS__)), fn), f, fn, sep, ##__VA_ARGS__))
#define REPEAT_F_SEP_ZERO(f, fn, sep, ...) EXPAND(REPEAT_N_F_SEP_ZERO(DIV(EXPAND(NUM(##__VA_ARGS__)), fn), f, fn, sep, ##__VA_ARGS__))
#define REVERSE_REPEAT_F_SEP(f, fn, sep, ...) EXPAND(REVERSE_REPEAT_N_F_SEP(DIV(EXPAND(NUM(##__VA_ARGS__)), fn), f, fn, sep, ##__VA_ARGS__))
#define REVERSE_REPEAT_F_SEP_ZERO(f, fn, sep, ...) EXPAND(REVERSE_REPEAT_N_F_SEP_ZERO(DIV(EXPAND(NUM(##__VA_ARGS__)), fn), f, fn, sep, ##__VA_ARGS__))

#define REPEAT_F(f, fn, ...) EXPAND(REPEAT_F_SEP(f, fn, COMMA_M, ##__VA_ARGS__))
#define REPEAT_F_ZERO(f, fn, ...) EXPAND(REPEAT_F_SEP_ZERO(f, fn, COMMA_M, ##__VA_ARGS__))
#define REVERSE_REPEAT_F(f, fn, ...) EXPAND(REVERSE_REPEAT_F_SEP(f, fn, COMMA_M, ##__VA_ARGS__))
#define REVERSE_REPEAT_F_ZERO(f, fn, ...) EXPAND(REVERSE_REPEAT_F_SEP_ZERO(f, fn, COMMA_M, ##__VA_ARGS__))

#define REPEAT_N(n, f, ...) EXPAND(REPEAT_N_F(n, f, 1, ##__VA_ARGS__))
#define REPEAT_N_ZERO(n, f, ...) EXPAND(REPEAT_N_F_ZERO(n, f, 1, ##__VA_ARGS__))
#define REVERSE_REPEAT_N(n, f, ...) EXPAND(REVERSE_REPEAT_N_F(n, f, 1, ##__VA_ARGS__))
#define REVERSE_REPEAT_N_ZERO(n, f, ...) EXPAND(REVERSE_REPEAT_N_F_ZERO(n, f, 1, ##__VA_ARGS__))

#define REPEAT(f, ...) EXPAND(REPEAT_F(f, 1, ##__VA_ARGS__))
#define REPEAT_ZERO(f, ...) EXPAND(REPEAT_F_ZERO(f, 1, ##__VA_ARGS__))
#define REVERSE_REPEAT(f, ...) EXPAND(REVERSE_REPEAT_F(f, 1, ##__VA_ARGS__))
#define REVERSE_REPEAT_ZERO(f, ...) EXPAND(REVERSE_REPEAT_F_ZERO(f, 1, ##__VA_ARGS__))

#define REPEAT_SEP(f, sep, ...) EXPAND(REPEAT_F_SEP(f, 1, sep, ##__VA_ARGS__))
#define REPEAT_SEP_ZERO(f, sep, ...) EXPAND(REPEAT_F_SEP_ZERO(f, 1, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_SEP(f, sep, ...) EXPAND(REVERSE_REPEAT_F_SEP(f, 1, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_SEP_ZERO(f, sep, ...) EXPAND(REVERSE_REPEAT_F_SEP_ZERO(f, 1, sep, ##__VA_ARGS__))
