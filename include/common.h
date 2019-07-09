#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#define IN
#define OUT
#define INOUT

typedef uint8_t byte;

typedef union _endian32 {
	uint32_t num;
	byte bytes[4];
}endian32;

typedef union _endian64 {
	uint64_t num;
	byte bytes[8];
}endian64;

#endif
