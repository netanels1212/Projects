#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* rand */
#include <stdio.h> /* printf */

#define ARR_SIZE 5000

typedef struct 
{
    size_t num;
    void *data;
} data_ty;

typedef enum
{
    IS_SORT = 0,
    IS_NOT_SORT = 1
} sort_status_ty;

typedef enum
{
    NO_ERROR = 0,
    ERROR = 1
} test_status_ty;


/*
undefined behavior: most_sig_bit_ <= 0 and least_sig_bit_ < 0,
                    least_sig_bit_ > most_sig_bit,
notes: healper function for CountSort() function
 */
int GetNumberFromBitsRange(size_t num_, int most_sig_bit_, int least_sig_bit_);


/*
undefined behavior: datas_, dest_, histogram_ NULL ptr.
                    most_sig_bit_ <= 0 and least_sig_bit_ < 0,
                    least_sig_bit_ > most_sig_bit,
                    size_ <= 0.
notes: dest size must be like the size of datas_ (size_),
       histogram size must be at least 2^((most_sig_bit_ - least_sig_bit_) + 1).
*/
void CountSort(data_ty datas_[], data_ty dest_[], size_t size_,
                     int most_sig_bit_, int least_sig_bit_, size_t histogram_[]);


/*
undefined behavior: datas_, dest_, histogram_ NULL ptr.
                    size_ <= 0,
                    num_important_bits_ <= 0,
                    bits_perpass_ <= 0.
                    bits_perpass_ > num_important_bits_.
notes: dest size must be like the size of datas_ (size_),
       histogram size must be at least 2^((most_sig_bit_ - least_sig_bit_) + 1).
*/
void RadixSort(data_ty datas_[], data_ty dest_[],size_t size_, 
             int num_important_bits_, size_t bits_perpass_, size_t histogram_[]);


/* 
function Description: initilaize the array with random numbers in the range of 
                      1 - 100.
undefined behavior: datas_ NULL pointer, len == 0.
*/
static void InitArr(data_ty datas_[], size_t len_);


/* 
function Description: check if the array is in sorted form.
undefined behavior: datas_ NULL pointer, len == 0
*/
static sort_status_ty SortChecking(data_ty datas_[], size_t len_);


static sort_status_ty RadixSortTest();



int main()
{
    sort_status_ty sort_status = IS_SORT;

    sort_status = RadixSortTest();
    
    if (IS_SORT != sort_status)
    {
        printf("\n *** There is an ERROR in the radix sort funtion ***\n");
        
        return ERROR;
    }
    
    printf("\n *** The radix sort function work properly ***\n");
    
    return NO_ERROR;
}


int GetNumberFromBitsRange(size_t num_, int most_sig_bit_, int least_sig_bit_)
{
    assert(0 <= least_sig_bit_);
    assert(0 < most_sig_bit_);
    assert(least_sig_bit_ < most_sig_bit_);

    return (~(~0 << (most_sig_bit_ - least_sig_bit_ + 1)) && 
                                                       (num_ >> least_sig_bit_));
}


void CountSort(data_ty datas_[], data_ty dest_[], size_t size_,
                      int most_sig_bit_, int least_sig_bit_, size_t histogram_[])
{
    int histogram_size = 0;
    int key = 0;
    int i = 0;

    /*assert*/
    assert(NULL != datas_);
    assert(NULL != dest_);
    assert(NULL != histogram_);
    assert(0 < size_);
    assert(0 <= least_sig_bit_);
    assert(0 < most_sig_bit_);
    assert(least_sig_bit_ < most_sig_bit_);

    histogram_size = (1U << ((most_sig_bit_ - least_sig_bit_) + 1)) - 1;          /* my TODO: why -1 ?? */

    /*nullify histogram*/
    for (i = 0; i < histogram_size; ++i)
    {
        histogram_[i] = 0;
    }

    /*for each element in datas_*/
        /*histogram[element_key - min_] += 1*/
    for (i = 0; (size_t)i < size_; ++i)
    {
        key = GetNumberFromBitsRange(datas_[i].num,most_sig_bit_,least_sig_bit_);
        histogram_[key] += 1;
    }

    /*for each element in histogram, from the second one*/
        /*histogram_[i] += histogram_[i - 1]*/
    for (i = 1; i < histogram_size; ++i)
    {
        histogram_[i] += histogram_[i - 1];
    }

    /*for each element in datas_*/
        /*dest_[histogram[element_key - min_] - 1] = element_key*/
        /*--histogram[element_key - min_]*/
    for (i = size_; i > 0; --i)
    {
        dest_[histogram_[(datas_[i - 1].num) - least_sig_bit_] - 1] = datas_[i - i];              /* my TODO: why -1 ?? */
        --histogram_[datas_[i - 1].num - least_sig_bit_];
    }
}


void RadixSort(data_ty datas_[], data_ty dest_[],size_t size_, 
              int num_important_bits_, size_t bits_perpass_, size_t histogram_[])
{
    /* assert */
    assert(NULL != datas_);
    assert(NULL != dest_);
    assert(NULL != histogram_);
    assert(0 < size_);
    assert(0 < num_important_bits_);
    assert(0 < bits_perpass_);
    assert(bits_perpass_ <= (size_t)num_important_bits_);
}

static void InitArr(data_ty datas_[], size_t len_)
{
    size_t i = 0;

    assert(NULL != datas_);
    assert(0 < len_);    

    for (i = 0; i < len_; ++i)
    {
        datas_[i].num = rand() % 100 + 1;
    }
}

static sort_status_ty SortChecking(data_ty datas_[], size_t len_)
{
    size_t i = 0;

    assert(NULL != datas_);
    assert(0 < len_);

    for (i = 0; i < len_ - 1; ++i)
    {
        if (datas_[i].num > datas_[i + 1].num)
        {
            return IS_NOT_SORT;
        }
    }

    return IS_SORT;
}

static sort_status_ty RadixSortTest()
{
    data_ty datas[ARR_SIZE];

    printf("\nTest for Radix Sort:\n");

    InitArr(datas, ARR_SIZE);

    return SortChecking(datas, ARR_SIZE);
}