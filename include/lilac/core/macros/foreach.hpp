#pragma once

#define LILAC_EXPAND(x) x
#define LILAC_FOR_EACH_1(what, delimiter, x, ...) what(x)
#define LILAC_FOR_EACH_2(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_1(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_3(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_2(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_4(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_3(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_5(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_4(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_6(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_5(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_7(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_6(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_8(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_7(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_9(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_8(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_10(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_9(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_11(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_10(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_12(what, delimiter, x, ...)\
  what(x) delimiter\
  LILAC_EXPAND(LILAC_FOR_EACH_11(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_13(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_12(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_14(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_13(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_15(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_14(what, delimiter, __VA_ARGS__))
#define LILAC_FOR_EACH_16(what, delimiter, x, ...)\
  what(x) delimiter \
  LILAC_EXPAND(LILAC_FOR_EACH_15(what, delimiter, __VA_ARGS__))

#define LILAC_FOR_EACH_NARG(...) LILAC_FOR_EACH_NARG_(__VA_ARGS__, LILAC_FOR_EACH_RSEQ_N())
#define LILAC_FOR_EACH_NARG_(...) LILAC_EXPAND(LILAC_FOR_EACH_ARG_N(__VA_ARGS__))
#define LILAC_FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define LILAC_FOR_EACH_RSEQ_N() 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define LILAC_CONCATENATE(x,y) x##y
#define LILAC_FOR_EACH_(N, what, delimiter, ...) LILAC_EXPAND(LILAC_CONCATENATE(LILAC_FOR_EACH_, N)(what, delimiter, __VA_ARGS__))


#define LILAC_FOR_EACH(what, delimiter, ...) LILAC_FOR_EACH_(LILAC_FOR_EACH_NARG(__VA_ARGS__), what, delimiter, __VA_ARGS__)
