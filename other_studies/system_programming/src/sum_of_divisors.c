#include <time.h> /* time_t, time */
#include <stdio.h> /* printf */


int sumOfDivisors(unsigned long num_)
{
    unsigned long sum = 0;
    unsigned long i = 0;
    
    for (i = 1; i <= (num_ / 2); ++i)
    {
        if (0 == (num_ % i))
        {
            sum += i;
        }
    }
    
    return sum;
}

int main()
{
	unsigned long num = 1000000000;
	unsigned long sum_of_divisors = 0;
	time_t start = time(NULL);
	time_t end;

	sum_of_divisors = sumOfDivisors(num);

	end = time(NULL);

	printf("Run time: %lu\n", end - start);

	printf("\nsum of divisors of %lu is: %lu\n", num, sum_of_divisors);

	return 0;
}