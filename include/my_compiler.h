#ifndef __MY__COMPILER__H
#define __MY__COMPILER__H
#if defined(__GNUC__)
#define PACKED __attribute__((__packed__))
#elif defined(__CC_ARM)
#define PACKED
#define inline __inline
#endif
#endif