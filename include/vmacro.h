#pragma once
#include "vmacroBase.h"

#define EXPAND(...) __VA_ARGS__
#define EXPAND_F(F, ...) EXPAND(F(__VA_ARGS__))

#define TO_STRING(X) TO_STRING1(X)
#define COMB(X, Y) COMB1(X, Y)

//分隔符
#define COMMA_M(X) ,X
#define SEM_M(X) ;X
#define EXCP_M(X) !X
#define OR_M(X) ||X
#define SPACE_M(X) X
#define UDRL_M(X) COMB(_, X)

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
#define CUTARGS_C(B, E, ...) EXPAND(CUTARGS(1, EXPAND(CUTARGS_B(RIS(B), X, ##__VA_ARGS__)), EXPAND(CUTARGS(RIS(E), X, ##__VA_ARGS__))))

//条件判断
#define SWITCH_CASE(conf, ...) EXPAND(GETARG(RIS(conf), ##__VA_ARGS__))
#define SWITCH_CASE_ARGS_R(conf, b, e, ...) EXPAND_F(COMB(GETARG_, RIS(conf))(RIS(conf), GETARG_F, SWITCH_CASE_ARGS_R_F, CUTARGS_C), b, e, ##__VA_ARGS__)
#define SWITCH_CASE_ARGS_N(conf, n, ...) EXPAND(SWITCH_CASE_ARGS_R(conf, 0, n, ##__VA_ARGS__))
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
//REPEAT系列开始---------------------------------------------------------------------------------------------------------------------------------------------------
#define REPEAT_N_F_A_SEP(n, f, fn, an, sep, ...) COMB(REPEAT_, n)(1, DIV(NUM(CUTARGS(an, ##__VA_ARGS__)), fn), n, f, fn, an, RIS, sep, ##__VA_ARGS__)
#define REPEAT_N_F_A_SEP_ZERO(n, f, fn, an, sep, ...) COMB(EXPAND_F(f, 0, CUTARGS_B(ADD(fn, an), CUTARGS_B(fn, CUTARGS(an, ##__VA_ARGS__)), CUTARGS_B(an, ##__VA_ARGS__))), sep(REPEAT_N_F_A_SEP(DEC(n), f, fn, an, sep, CUTARGS_C(an, ADD(an, fn), ##__VA_ARGS__))))
#define REVERSE_REPEAT_N_F_A_SEP(n, f, fn, an, sep, ...) COMB(REPEAT_, n)(n, DIV(NUM(CUTARGS(an, ##__VA_ARGS__)), fn), n, f, fn, an, DEC, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_A_SEP_ZERO(n, f, fn, an, sep, ...) COMB(REVERSE_REPEAT_N_F_A_SEP(DEC(n), f, fn, an, sep, ##__VA_ARGS__), sep(EXPAND_F(f, 0, CUTARGS_B(fn, CUTARGS(ADD(an, MUL(fn, DEC(MIN(n, DIV(NUM(CUTARGS(an, ##__VA_ARGS__)), fn))))), ##__VA_ARGS__)), CUTARGS_B(an, ##__VA_ARGS__)))) 

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
#define REPEAT_N_F_A(n, f, fn, an, ...) REPEAT_N_F_A_SEP(n, f, fn, an, COMMA_M, ##__VA_ARGS__)
#define REPEAT_N_F_A_ZERO(n, f, fn, an, ...) REPEAT_N_F_A_SEP_ZERO(n, f, fn, an, COMMA_M, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_A(n, f, fn, an, ...) REVERSE_REPEAT_N_F_A_SEP(n, f, fn, an, COMMA_M, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_A_ZERO(n, f, fn, an, ...) REVERSE_REPEAT_N_F_A_SEP_ZERO(n, f, fn, an, COMMA_M, ##__VA_ARGS__)

#define REPEAT_N_F_SEP(n, f, fn, sep, ...) REPEAT_N_F_A_SEP(n, f, fn, 0, sep, ##__VA_ARGS__)
#define REPEAT_N_F_SEP_ZERO(n, f, fn, sep, ...) REPEAT_N_F_A_SEP_ZERO(n, f, fn, 0, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_SEP(n, f, fn, sep, ...) REVERSE_REPEAT_N_F_A_SEP(n, f, fn, 0, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_SEP_ZERO(n, f, fn, sep, ...) REVERSE_REPEAT_N_F_A_SEP_ZERO(n, f, fn, 0, sep, ##__VA_ARGS__)

#define REPEAT_N_A_SEP(n, f, an, sep, ...) REPEAT_N_F_A_SEP(n, f, 1, an, sep, ##__VA_ARGS__)
#define REPEAT_N_A_SEP_ZERO(n, f, an, sep, ...) REPEAT_N_F_A_SEP_ZERO(n, f, 1, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_A_SEP(n, f, an, sep, ...) REVERSE_REPEAT_N_F_A_SEP(n, f, 1, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_A_SEP_ZERO(n, f, an, sep, ...) REVERSE_REPEAT_N_F_A_SEP_ZERO(n, f, 1, an, sep, ##__VA_ARGS__)

#define REPEAT_F_A_SEP(f, fn, an, sep, ...) REPEAT_N_F_A_SEP(DIV(EXPAND(NUM(CUTARGS(an, ##__VA_ARGS__))), fn), f, fn, an, sep, ##__VA_ARGS__)
#define REPEAT_F_A_SEP_ZERO(f, fn, an, sep, ...) REPEAT_N_F_A_SEP_ZERO(DIV(EXPAND(NUM(CUTARGS(an, ##__VA_ARGS__))), fn), f, fn, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_F_A_SEP(f, fn, an, sep, ...) REVERSE_REPEAT_N_F_A_SEP(DIV(EXPAND(NUM(CUTARGS(an, ##__VA_ARGS__))), fn), f, fn, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_F_A_SEP_ZERO(f, fn, an, sep, ...) REVERSE_REPEAT_N_F_A_SEP_ZERO(DIV(EXPAND(NUM(CUTARGS(an, ##__VA_ARGS__))), fn), f, fn, an, sep, ##__VA_ARGS__)

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
#define REPEAT_N_F(n, f, fn, ...) REPEAT_N_F_A(n, f, fn, 0, ##__VA_ARGS__)
#define REPEAT_N_F_ZERO(n, f, fn, ...) REPEAT_N_F_A_ZERO(n, f, fn, 0, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F(n, f, fn, ...) REVERSE_REPEAT_N_F_A(n, f, fn, 0, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_F_ZERO(n, f, fn, ...) REVERSE_REPEAT_N_F_A_ZERO(n, f, fn, 0, ##__VA_ARGS__)

#define REPEAT_N_A(n, f, an, ...) REPEAT_N_F_A(n, f, 1, an, ##__VA_ARGS__)
#define REPEAT_N_A_ZERO(n, f, an, ...) REPEAT_N_F_A_ZERO(n, f, 1, an, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_A(n, f, an, ...) REVERSE_REPEAT_N_F_A(n, f, 1, an, ##__VA_ARGS__)
#define REVERSE_REPEAT_N_A_ZERO(n, f, an, ...) REVERSE_REPEAT_N_F_A_ZERO(n, f, 1, an, ##__VA_ARGS__)

#define REPEAT_N_SEP(n, f, sep, ...) REPEAT_N_F_SEP(n, f, 1, sep, ##__VA_ARGS__)								  //
#define REPEAT_N_SEP_ZERO(n, f, sep, ...) REPEAT_N_F_SEP_ZERO(n, f, 1, sep, ##__VA_ARGS__)						  //
#define REVERSE_REPEAT_N_SEP(n, f, sep, ...) REVERSE_REPEAT_N_F_SEP(n, f, 1, sep, ##__VA_ARGS__)				  //
#define REVERSE_REPEAT_N_SEP_ZERO(n, f, sep, ...) REVERSE_REPEAT_N_F_SEP_ZERO(n, f, 1, sep, ##__VA_ARGS__)		  //

#define REPEAT_F_A(f, fn, an, ...) REPEAT_F_A_SEP(f, fn, an, COMMA_M, ##__VA_ARGS__)							  //
#define REPEAT_F_A_ZERO(f, fn, an, ...) REPEAT_F_A_SEP_ZERO(f, fn, an, COMMA_M, ##__VA_ARGS__)					  //
#define REVERSE_REPEAT_F_A(f, fn, an, ...) REVERSE_REPEAT_F_A_SEP(f, fn, an, COMMA_M, ##__VA_ARGS__)			  //
#define REVERSE_REPEAT_F_A_ZERO(f, fn, an, ...) REVERSE_REPEAT_F_A_SEP_ZERO(f, fn, an, COMMA_M, ##__VA_ARGS__)	  

#define REPEAT_F_SEP(f, fn, sep, ...) REPEAT_F_A_SEP(f, fn, 0, sep, ##__VA_ARGS__)								  //
#define REPEAT_F_SEP_ZERO(f, fn, sep, ...) REPEAT_F_A_SEP_ZERO(f, fn, 0, sep, ##__VA_ARGS__)					  //
#define REVERSE_REPEAT_F_SEP(f, fn, sep, ...) REVERSE_REPEAT_F_A_SEP(f, fn, 0, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_F_SEP_ZERO(f, fn, sep, ...) REVERSE_REPEAT_F_A_SEP_ZERO(f, fn, 0, sep, ##__VA_ARGS__)

#define REPEAT_A_SEP(f, an, sep, ...) REPEAT_F_A_SEP(f, 1, an, sep, ##__VA_ARGS__)								  //
#define REPEAT_A_SEP_ZERO(f, an, sep, ...) REPEAT_F_A_SEP_ZERO(f, 1, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_A_SEP(f, an, sep, ...) REVERSE_REPEAT_F_A_SEP(f, 1, an, sep, ##__VA_ARGS__)
#define REVERSE_REPEAT_A_SEP_ZERO(f, an, sep, ...) REVERSE_REPEAT_F_A_SEP_ZERO(f, 1, an, sep, ##__VA_ARGS__)

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
#define REPEAT_N(n, f, ...) REPEAT_N_F(n, f, 1, ##__VA_ARGS__) 													  //
#define REPEAT_N_ZERO(n, f, ...) REPEAT_N_F_ZERO(n, f, 1, ##__VA_ARGS__) 										  //
#define REVERSE_REPEAT_N(n, f, ...) REVERSE_REPEAT_N_F(n, f, 1, ##__VA_ARGS__) 									  //
#define REVERSE_REPEAT_N_ZERO(n, f, ...)REVERSE_REPEAT_N_F_ZERO(n, f, 1, ##__VA_ARGS__)							  //

#define REPEAT_F(f, fn, ...) REPEAT_F_SEP(f, fn, COMMA_M, ##__VA_ARGS__) 										  //
#define REPEAT_F_ZERO(f, fn, ...) REPEAT_F_SEP_ZERO(f, fn, COMMA_M, ##__VA_ARGS__) 								  //
#define REVERSE_REPEAT_F(f, fn, ...) REVERSE_REPEAT_F_SEP(f, fn, COMMA_M, ##__VA_ARGS__) 						  //
#define REVERSE_REPEAT_F_ZERO(f, fn, ...)REVERSE_REPEAT_F_SEP_ZERO(f, fn, COMMA_M, ##__VA_ARGS__)				  //

#define REPEAT_A(f, an, ...) REPEAT_A_SEP(f, an, COMMA_M, ##__VA_ARGS__) 										  //
#define REPEAT_A_ZERO(f, an, ...) REPEAT_A_SEP_ZERO(f, an, COMMA_M, ##__VA_ARGS__)								  //
#define REVERSE_REPEAT_A(f, an, ...) REVERSE_REPEAT_A_SEP(f, an, COMMA_M, ##__VA_ARGS__) 
#define REVERSE_REPEAT_A_ZERO(f, an, ...)REVERSE_REPEAT_A_SEP_ZERO(f, an, COMMA_M, ##__VA_ARGS__)

#define REPEAT_SEP(f, sep, ...) REPEAT_A_SEP(f, 0, sep, ##__VA_ARGS__) 											  //
#define REPEAT_SEP_ZERO(f, sep, ...) REPEAT_A_SEP_ZERO(f, 0, sep, ##__VA_ARGS__)								  //
#define REVERSE_REPEAT_SEP(f, sep, ...) REVERSE_REPEAT_A_SEP(f, 0, sep, ##__VA_ARGS__) 							  //
#define REVERSE_REPEAT_SEP_ZERO(f, sep, ...)REVERSE_REPEAT_A_SEP_ZERO(f, 0, sep, ##__VA_ARGS__)					  //

#define REPEAT(f, ...) REPEAT_SEP(f, COMMA_M, ##__VA_ARGS__) 													  //
#define REPEAT_ZERO(f, ...) REPEAT_SEP_ZERO(f, COMMA_M, ##__VA_ARGS__) 											  //
#define REVERSE_REPEAT(f, ...) REVERSE_REPEAT_SEP(f, COMMA_M, ##__VA_ARGS__) 									  //
#define REVERSE_REPEAT_ZERO(f, ...) REVERSE_REPEAT_SEP_ZERO(f, COMMA_M, ##__VA_ARGS__) 							  

//REPEAT系列结束---------------------------------------------------------------------------------------------------------------------------------------------------