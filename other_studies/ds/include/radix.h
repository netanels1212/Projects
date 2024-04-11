typedef struct 
{
    size_t num;
    void *data;
} data_ty;

data_ty datas[5000];


/*histogram size must be 2^bits_perpass*/
/*numbers in datas must be unsigned*/
void RadixSort(data_ty datas_[], data_ty dest_[],size_t size_, 
             int num_important_bits_, size_t bits_perpass_, size_t histogram_[]);