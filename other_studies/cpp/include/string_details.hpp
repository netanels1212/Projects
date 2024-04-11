#ifndef ILRD_HRD33_DETAILS_HPP
#define ILRD_HRD33_DETAILS_HPP

#include <cstddef> // size_t

#include "utils_cpp.hpp"

namespace details
{
class CountedResource
{
public:
    static CountedResource* Create(const char* s_);
    char *GetStr();
    void IncrementCounter();
    void DecrementCounter();
    CountedResource* SplitResource();
private:
    CountedResource(const char *str_);
    ~CountedResource();
    size_t m_counter;
    char m_str[1];
    CountedResource(CountedResource &other_);//Do not implement
    CountedResource &operator=(const CountedResource &other_); //Do not implement
};
}//end of details
#endif //ILRD_HRD33_DETAILS_HPP