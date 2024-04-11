#include <stddef.h> /* size_t */
#include <time.h> /* time_t, time */
#include <unistd.h> /* getpid */
#include <pthread.h> /* threads functions */

#include "uid.h"

enum {IS_NOT_SAME = 0, IS_SAME = 1};

uid_ty bad_uid = {0, 0, 0};

pthread_mutex_t g_counter_mutex;


uid_ty UIDGenerate(void)
{
	uid_ty uid;
	static size_t proc_counter = 0;
	
	uid.pid = getpid();
	uid.time_stamp = time(NULL);

	pthread_mutex_lock(&g_counter_mutex); 

	uid.id_counter = proc_counter;
	
	++proc_counter;

	pthread_mutex_unlock(&g_counter_mutex); 

	
	return uid;
}

int UIDIsSame(uid_ty first, uid_ty second)
{	
	if ((first.pid != second.pid) || (first.time_stamp != second.time_stamp) ||
	    (first.id_counter != second.id_counter))
	{
		return IS_NOT_SAME;
	}
	
	return IS_SAME;
}

uid_ty UIDGetBadUID(void)
{
	return bad_uid;
}
