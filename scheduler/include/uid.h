/*******************************************************************************
Project: UID
Name: Netanel Shmuel
Reviewer: 
Date: 12/11/23
Version: 1.0
*******************************************************************************/

#ifndef __UID_H__
#define __UID_H__

#include <stddef.h> /* size_t */
#include <time.h> /* time_t */

typedef struct uid uid_ty;

extern uid_ty bad_uid;


/*
Function Description: Generate unique id.

Return value: uid struct by val 

Undefined behaviour: None

Time Complexity: O(1)

on failure: BADID.
*/
uid_ty UIDGenerate(void);


/*
Function Description: compare id.

Return value: true(1) or false(0)

Undefined behaviour: None

Time Complexity: O(1)

on failure: NULL.
*/
int UIDIsSame(uid_ty first, uid_ty second);


/*
Function Description: return bad UID.
	
Return value: bad UID value

Undefined behaviour: None

Time Complexity: O(1)
	
on failure: none.
*/
uid_ty UIDGetBadUID(void);











/*No bad touch*/

struct uid
{
	size_t pid;
	time_t time_stamp;
	size_t id_counter;
};

#endif /*__UID_H__*/



