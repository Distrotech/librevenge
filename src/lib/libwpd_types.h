#ifndef LIBWPD_TYPES_H
#define LIBWPD_TYPES_H

#if defined (__GNUC__) || defined (GCC)
#include <stdint.h>
#endif
#ifdef _MSC_VER
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#endif

#endif /* LIBWPD_TYPES_H */
