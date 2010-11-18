#ifndef COAL_INTERNAL_H
#define COAL_INTERNAL_H

#define __kernel

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

//typedef float float4 __attribute__((ext_vector_type(4)));
#define COAL_VECTOR(type, len)                                  \
   typedef type type##len __attribute__((ext_vector_type(len)))
#define COAL_VECTOR_SET(type) \
   COAL_VECTOR(type, 2);      \
   COAL_VECTOR(type, 3);      \
   COAL_VECTOR(type, 4)

COAL_VECTOR_SET(char);
COAL_VECTOR_SET(uchar);

COAL_VECTOR_SET(short);
COAL_VECTOR_SET(ushort);

COAL_VECTOR_SET(int);
COAL_VECTOR_SET(uint);

COAL_VECTOR_SET(long);
COAL_VECTOR_SET(ulong);

COAL_VECTOR_SET(float);

#undef COAL_VECTOR_SET
#undef COAL_VECTOR

#define __global __attribute__((address_space(1)))
#define __local __attribute__((address_space(2)))
#define __constant __attribute__((address_space(3)))
#define __private __attribute__((address_space(4)))


extern int get_global_id(int);

#endif
