/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002,2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef LIBWPD_INTERNAL_H
#define LIBWPD_INTERNAL_H
#include "WPXStream.h"
#include <stdio.h>
#include "libwpd_support.h"

/* Convenience functions/defines, should not be exported externally */

#define WPD_CHECK_FILE_ERROR(v) if (v==EOF) { WPD_DEBUG_MSG(("X_CheckFileError: %d\n", __LINE__)); throw FileException(); }
#define WPD_CHECK_FILE_SEEK_ERROR(v) if (v) { WPD_DEBUG_MSG(("X_CheckFileSeekError: %d\n", __LINE__)); throw FileException(); }
#define WPD_CHECK_FILE_READ_ERROR(v,num_elements) if (v != num_elements) {\
 WPD_DEBUG_MSG(("X_CheckFileReadElementError: %d\n", __LINE__)); throw FileException(); }

#define DELETEP(m) if (m) { delete m; m = NULL; }
 
#ifdef DEBUG
#define WPD_DEBUG_MSG(M) printf M
#else
#define WPD_DEBUG_MSG(M)
#endif

#define WPD_LE_GET_GUINT8(p) (*(uint8_t const *)(p))
#define WPD_LE_GET_GUINT16(p)                           \
        (uint16_t)((((uint8_t const *)(p))[0] << 0)  |    \
                  (((uint8_t const *)(p))[1] << 8))
#define WPD_LE_GET_GUINT32(p)                           \
        (uint32_t)((((uint8_t const *)(p))[0] << 0)  |    \
                  (((uint8_t const *)(p))[1] << 8)  |    \
                  (((uint8_t const *)(p))[2] << 16) |    \
                  (((uint8_t const *)(p))[3] << 24))

#define WPD_BE_GET_GUINT8(p) (*(uint8_t const *)(p))
#define WPD_BE_GET_GUINT16(p)                           \
        (uint16_t)((((uint8_t const *)(p))[1] << 0)  |    \
                  (((uint8_t const *)(p))[0] << 8))
#define WPD_BE_GET_GUINT32(p)                           \
        (uint32_t)((((uint8_t const *)(p))[3] << 0)  |    \
                  (((uint8_t const *)(p))[2] << 8)  |    \
                  (((uint8_t const *)(p))[1] << 16) |    \
                  (((uint8_t const *)(p))[0] << 24))

// add more of these as needed for byteswapping
// (the 8-bit functions are just there to make things consistent)
int8_t read8(WPXInputStream *input); 
uint8_t readU8(WPXInputStream *input); 
uint16_t readU16(WPXInputStream *input, bool bigendian=false);
uint32_t readU32(WPXInputStream *input, bool bigendian=false);

#endif /* LIBWPD_INTERNAL_H */
