#include <stdio.h> /* printf, scanf */
#include <time.h> /* time */
#include<unistd.h> /* sleep */

int main()
{
	time_t start_time = time(NULL); 
	time_t end_time = time(NULL);

	while(start_time + 5 > end_time)
	{
		sleep(1);

		end_time = time(NULL);

		printf("second - %lu \n", (end_time - start_time));
	}
	
	return 0;
}