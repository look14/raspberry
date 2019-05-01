#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define BOOL  u8
#define TRUE  1
#define FALSE 0

#define __min(x,y)	(((x)<(y)) ?(x) :(y))
#define __max(x,y)	(((x)>(y)) ?(x) :(y))

#define INFINITE	0xFFFFFFFFUL

#endif

