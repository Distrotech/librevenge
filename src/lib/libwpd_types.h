#ifndef LIBWPD_TYPES_H
#define LIBWPD_TYPES_H

#ifdef _MSC_VER
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#else /* _MSC_VER */
#include <inttypes.h>
#endif /* _MSC_VER */

#endif /* LIBWPD_TYPES_H */
