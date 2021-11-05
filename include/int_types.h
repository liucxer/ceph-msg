#ifndef CEPH_INTTYPES_H
#define CEPH_INTTYPES_H

#include "include/acconfig.h"

#include <inttypes.h>

#ifdef HAVE_LINUX_TYPES_H
#include <linux/types.h>
#else
#ifndef HAVE___U8
typedef uint8_t uint8_t;
#endif

#ifndef HAVE___S8
typedef int8_t int8_t;
#endif

#ifndef HAVE___U16
typedef uint16_t uint16_t;
#endif

#ifndef HAVE___S16
typedef int16_t int16_t;
#endif

#ifndef HAVE___U32
typedef uint32_t uint32_t;
#endif

#ifndef HAVE___S32
typedef int32_t int32_t;
#endif

#ifndef HAVE___U64
typedef uint64_t uint64_t;
#endif

#ifndef HAVE___S64
typedef int64_t __s64;
#endif
#endif /* LINUX_TYPES_H */

#define __bitwise__

typedef uint16_t __bitwise__ __le16;
typedef uint16_t __bitwise__ __be16;
typedef uint32_t __bitwise__ __le32;
typedef uint32_t __bitwise__ __be32;
typedef uint64_t __bitwise__ __le64;
typedef uint64_t __bitwise__ __be64;

#ifndef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif

#ifndef BOOST_MPL_LIMIT_VECTOR_SIZE
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30 // or whatever you need
#endif

#ifndef BOOST_MPL_LIMIT_MAP_SIZE
#define BOOST_MPL_LIMIT_MAP_SIZE 30 // or whatever you need
#endif

#endif
