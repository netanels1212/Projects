#ifndef UTILS_H
#define UTILS_H

#if __cplusplus >= 201103L

#define NOEXCEPT noexcept(false)

#define NULLPTR nullptr

#else

#define NOEXCEPT throw()

#define NULLPTR 0

#endif // __cplusplus >= 201103L

#endif // UTILS_H
