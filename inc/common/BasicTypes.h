#ifndef __MWL_BASIC_TYPES_H__
#define __MWL_BASIC_TYPES_H__ 

#if _MSC_VER < 1900
	typedef signed char int8_t;
	typedef unsigned char uint8_t;
	typedef signed short int16_t;
	typedef unsigned short uint16_t;
	typedef signed int int32_t;
	typedef unsigned int uint32_t;
	typedef signed long long int64_t;
	typedef unsigned long long uint64_t;
#else
	#include <stdint.h>
#endif

#endif // __MWL_BASIC_TYPES_H__

