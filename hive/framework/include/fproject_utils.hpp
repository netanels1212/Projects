#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/types.h>
#include <netinet/in.h>

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a)
{
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) 
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

#endif // UTILS_HPP