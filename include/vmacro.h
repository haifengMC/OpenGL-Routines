#pragma once

#define TO_STRING(X) TO_STRING1(X)
#define TO_STRING1(X) #X

#define COMB(X, Y) COMB1(X, Y)
#define COMB1(X, Y) X##Y

#define COMMA(X) ,X

#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC(n) COMB(DEC_, n)

#define REPEAT_1(n, f, e) f(n, e)
#define REPEAT_2(n, f, e) COMB(COMB(REPEAT_, DEC(n))(DEC(n), f, e), COMMA(f(n, e)))
#define REPEAT_3(n, f, e) COMB(COMB(REPEAT_, DEC(n))(DEC(n), f, e), COMMA(f(n, e)))
