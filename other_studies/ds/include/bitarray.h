/******************************************************************************/
/*   Project: Word Boundary
	 Name: Netanel Shmuel
	 Reviewer: Zaher 
	 date: 28.9.2023
	 version: 1.0
*/
/******************************************************************************/
#ifndef __BITARRAY_H__
#define __BITARRAY_H__

#include <stddef.h> /* size_t */

#define EXIT 0
#define EQUAL 0
#define BYTE 8
#define MAX_STR_SIZE 80

enum {SUCCESS, FAIL};

typedef size_t bitarr_ty;

/* Return value: bit array with all bits set to '1' */
bitarr_ty SetAll();

/* Return value: bit array with all bits set to '0' */
bitarr_ty ResetAll();

/* Return value: The new bit array after set the bit in this index to '1'.
Undefined behaviour: index bigger then 63 */
bitarr_ty SetOn(bitarr_ty arr, size_t index);

/* Return value: The new bit array after set the bit in this index to '1'.
Undefined behaviour: index bigger then 63 */
bitarr_ty SetOff(bitarr_ty arr, size_t index);

/* Return value: The new bit array after set the bit in this index
                 to '1' or '0' depending on the boolean_value.
Undefined behaviour: index bigger then 63, value not '0' or '1' */
bitarr_ty SetBit(bitarr_ty arr, size_t index, int boolean_value);

/* Return value: The value of the bit in this index.
Undefined behaviour: index bigger then 63 */
int GetVal(bitarr_ty arr, size_t index);

/* Return value: '1' if the bit in this index is '0'.
				  Otherwise, return '0'.
Undefined behaviour: index bigger then 63 */
int BitIsOff(bitarr_ty arr, size_t index);

/* Return value: The new bit array after flip the bit in this index.
Undefined behaviour: index bigger then 63 */
bitarr_ty FlipBit(bitarr_ty arr, size_t index);

/* Return value: The new bit array after mirror */
bitarr_ty Mirror(bitarr_ty arr);

/* Return value: The new bit array after mirror. implement with LUT */
bitarr_ty BitsArrLutMirror(bitarr_ty arr);

/* Return value: The new bit array after rotate right.
Undefined behaviour: shift bigger then 64 */
bitarr_ty RotR(bitarr_ty arr, size_t shift);

/* Return value: The new bit array after rotate left.
Undefined behaviour: shift bigger then 64 */
bitarr_ty RotL(bitarr_ty arr, size_t shift);

/* Return value: The size of set bits in the bit array */
size_t CountOn(bitarr_ty arr);

/* Return value: The size of off bits in the bit array */
size_t CountOff(bitarr_ty arr);

/* Return value: The size of set bits in the bit array. implement with LUT */
size_t BitsArrLutCountOn(bitarr_ty arr);

/* Return value: Pointer to the string represent the bit array.
Undefined behaviour: NULL ptr */
char *ToString(bitarr_ty arr, char *dest);

#endif /* __BITARRAY_H__ */
