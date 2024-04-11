#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>/*fputs*/

#ifdef _DEBUG
#define DEBUG_ONLY(X) X
#else
#define DEBUG_ONLY(X)
#endif /* _DEBUG */


#define BAD_MEM(type) ( (type)0xBADC0FFEE0DDF00D )

#define RET_IF_BAD(IS_GOOD, RET_VAL) if (!(IS_GOOD)) { DEBUG_ONLY(fprintf(stderr,"RET_IF_BAD failed with: " #IS_GOOD) ;) return RET_VAL; }

#endif /*__UTILS_H__*/