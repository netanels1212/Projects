/*****************************************************************************/
/*
Project: BitArray
Name: Netanel shemuel
Reviewer: Yan
Date: 26.02.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_BITARRAY_HPP
#define ILRD_HRD33_BITARRAY_HPP

#include <cstddef> //size_t
#include <cstring> //memset
#include <string> //string
#include <algorithm> //fill, transform
#include <numeric> //accumulate

namespace ilrd_hrd33_bitarray
{
const size_t bit_count = __CHAR_BIT__ * sizeof(size_t); //TODO: constant in capital letters

const unsigned char set_bits_lut_g[256] = //TODO: macro function to do it in compile time
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

//TODO: all the halper function not in a different namespace, or static member function in the right class
static size_t CountBitsIMP(size_t total, size_t val)
{
    //TODO: assert that val not bigger then LUT size
    return total + set_bits_lut_g[static_cast<unsigned char>(val)];
}

std::string ToString(size_t num)
{
    std::string string;

    const int num_bits = sizeof(size_t) * __CHAR_BIT__;

    for (int i = num_bits - 1; i >= 0; --i) //TODO: don't use for, do iterator to bit using operator[]
    {
        string += '0' + ((num >> i) & 1);
    }

    return string;
}

struct To_String
{
    std::string operator()(const std::string &result, size_t element) const
    {
        return result + ToString(element);
    }
};

//TODO: you can combaine those 2 funcs to one, be sending reference to arrIdx and bitPos
static size_t ArrIdxIMP(size_t idx_)
{
    return idx_ / bit_count;
}

static size_t BitPosIMP(size_t idx_)
{
    return idx_ % bit_count;
}

template <size_t SIZE> //TODO: "SIZE" - do a more descriptive name
class BitArray //Uses generated dtor, CCtor, Assignment Ctor
{
private:
    class BitProxy;
public:
    explicit BitArray();                
    bool operator[](size_t idx_) const; //Exception - std::out_of_range   TODO: no need to throw exception
    BitProxy operator[](size_t idx_);   //Exception - std::out_of_range
    BitArray<SIZE> &operator|=(BitArray<SIZE> &o_);
    BitArray<SIZE> &operator&=(BitArray<SIZE> &o_);
    BitArray<SIZE> &operator^=(BitArray<SIZE> &o_);
    BitArray<SIZE> &operator<<=(size_t size);
    BitArray<SIZE> &operator>>=(size_t size);
    bool operator==(const BitArray<SIZE> &o_) const;
    bool operator!=(const BitArray<SIZE> &o_) const;
    BitArray<SIZE> &Set(size_t idx_); //Exception - std::out_of_range
    BitArray<SIZE> &Set();
    bool Get(size_t idx_) const; //Exception - std::out_of_range
    BitArray<SIZE> &Flip(size_t idx_); //Exception - std::out_of_range
    BitArray<SIZE> &Flip();
    size_t CountBits();
    std::string ToString() const;
private:
    size_t m_arr[(SIZE + bit_count -1 ) / bit_count]; //TODO: do the calc in const var, and use member size
    static const size_t size; //TODO: s_size?
    class BitProxy //Uses generated dtor, cctor
    {
    public:
        explicit BitProxy(BitArray<SIZE> &arr_, size_t idx_);
        bool operator=(bool o_);
        bool operator=(const BitProxy &o_);
        operator bool() const;
    private:
        BitArray &arr; //TODO: m_arr
        const size_t index; //TODO: m_index
    };
};

template <size_t SIZE>
size_t BitArray<SIZE>::size = SIZE; //TODO: no need to be initialize here

template <size_t SIZE>
BitArray<SIZE>::BitArray()
{
    std::fill(m_arr, m_arr + (SIZE + bit_count - 1) / bit_count, 0);
}

template <size_t SIZE>
bool BitArray<SIZE>::operator[](size_t idx_) const
{
    size_t array_idx = ArrIdxIMP(idx_);
    size_t bit_pos = BitPosIMP(idx_);

    return (m_arr[array_idx] >> (bit_count - bit_pos - 1)) & 1UL; //TODO: this calc repeat on itself - const or halper func
}

template <size_t SIZE>
typename BitArray<SIZE>::BitProxy BitArray<SIZE>::operator[](size_t idx_)
{
    return BitProxy(*this, idx_);
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator|=(BitArray<SIZE> &o_)
{
    std::transform(m_arr, m_arr + ((SIZE + bit_count -1 ) / bit_count), o_.m_arr, m_arr, std::bit_or<size_t>()); //TODO: all those functors bit_or etc. - implement by yourself
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator&=(BitArray<SIZE> &o_)
{
    std::transform(m_arr, m_arr + ((SIZE + bit_count -1 ) / bit_count), o_.m_arr, m_arr, std::bit_and<size_t>());
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator^=(BitArray<SIZE> &o_)
{
    std::transform(m_arr, m_arr + ((SIZE + bit_count -1 ) / bit_count), o_.m_arr, m_arr, std::bit_xor<size_t>());
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator<<=(size_t size) //TODO
{
    return 0;
}

template <size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator>>=(size_t size) //TODO
{
    return 0;
}

template <size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray<SIZE> &o_) const
{
    return std::equal(m_arr, m_arr + (SIZE + bit_count - 1) / bit_count, o_.m_arr); //TODO: bug! - if the bits that are not in the size of the bitarray are dirty
}

template <size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray<SIZE> &o_) const
{
    return !(*this == o_);
}

template <size_t SIZE>
BitArray<SIZE> &BitArray<SIZE>::Set(size_t idx_) //TODO: use operator[] - code reuse
{
    if (idx_ > SIZE)
    {
        throw std::out_of_range("in Set function: idx_ is out of bound.");
    }

    size_t array_idx = ArrIdxIMP(idx_);
    size_t bit_pos = BitPosIMP(idx_);

    m_arr[array_idx] |= (1UL << (bit_count - bit_pos - 1));   
}

template <size_t SIZE>
BitArray<SIZE> &BitArray<SIZE>::Set() 
{
    std::fill(m_arr, m_arr + (SIZE + bit_count - 1) / bit_count, ~0UL);

    return *this;
}

template <size_t SIZE>
bool BitArray<SIZE>::Get(size_t idx_) const //TODO: use operator[] - code reuse
{
    if (idx_ > SIZE) //TODO: use s_size and not SIZE when you can in all the code
    {
        throw std::out_of_range("in Get function: idx_ is out of bound.");
    }

    size_t array_idx = ArrIdxIMP(idx_);
    size_t bit_pos = BitPosIMP(idx_);

    return (m_arr[array_idx] >> (bit_count - bit_pos - 1)) & 1UL;
}

template <size_t SIZE>
BitArray<SIZE> &BitArray<SIZE>::Flip(size_t idx_) //TODO: use operator[] - code reuse
{
    if (idx_ > SIZE)
    {
        throw std::out_of_range("in Flip function: idx_ is out of bound.");
    }

    size_t array_idx = ArrIdxIMP(idx_);
    size_t bit_pos = BitPosIMP(idx_);
    
    m_arr[array_idx] ^= (1UL << (bit_count - bit_pos - 1));
}

template <size_t SIZE>
BitArray<SIZE> &BitArray<SIZE>::Flip()
{
    std::transform(m_arr, m_arr + (SIZE + bit_count - 1) / bit_count, m_arr, std::bit_not<size_t>());

    return *this;

}

template <size_t SIZE>
size_t BitArray<SIZE>::CountBits() //TODO: do const, and use const_cast and clean the extra bits
{
    m_arr[((SIZE + bit_count - 1) / bit_count) - 1] &= ((1UL << SIZE % bit_count ) - 1UL);

    return std::accumulate(reinterpret_cast<const char *>(m_arr), 
    reinterpret_cast<const char *>((m_arr) + (SIZE + bit_count - 1) / bit_count),
    0, CountBitsIMP);
}

template <size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    std::string ret = std::accumulate(m_arr, m_arr + (SIZE + bit_count - 1) / bit_count, std::string(), To_String());

    if(ret.size()>SIZE)
    {
        ret.erase(SIZE);
    }

    std::reverse(ret.begin(), ret.end()); //TODO: use reverse iterator - iterate from end to start
    
    return ret;
}

template <size_t SIZE>
BitArray<SIZE>::BitProxy::BitProxy(BitArray<SIZE> &arr_, size_t idx_) : arr(arr_) , index(idx_)
{

}

template <size_t SIZE>
bool BitArray<SIZE>::BitProxy::operator=(bool o_)
{
    size_t array_idx = ArrIdxIMP(index);
    size_t bit_pos = BitPosIMP(index);

    bool current_value = (arr.m_arr[array_idx] >> (bit_count - bit_pos - 1)) & 1UL;

    if (current_value != o_) 
    {
        arr.m_arr[array_idx] ^= (1UL << (bit_count - bit_pos - 1)); //TODO: you can set off the bit, and then set him with right value
    }

    return o_;
}

template <size_t SIZE>
bool BitArray<SIZE>::BitProxy::operator=(const BitProxy &o_) //TODO: code reuse with other operator=
{
    size_t array_idx = ArrIdxIMP(index);
    size_t bit_pos = BitPosIMP(index);

    size_t o_array_idx = ArrIdxIMP(o_.index);
    size_t o_bit_pos = BitPosIMP(o_.index);

    bool current_value = (arr.m_arr[array_idx] >> (bit_count - bit_pos - 1)) & 1UL;
    bool other_value = (o_.arr.m_arr[o_array_idx] >> (bit_count - bit_pos - 1)) & 1UL;

    if (current_value != other_value) 
    {
        arr.m_arr[array_idx] ^= (1UL << (bit_count - bit_pos - 1));
    }

    return other_value;
}

template <size_t SIZE>
BitArray<SIZE>::BitProxy::operator bool() const
{
    size_t array_idx = ArrIdxIMP(index);
    size_t bit_pos = BitPosIMP(index);

    return (arr.m_arr[array_idx] & (1 << (bit_count - bit_pos - 1))) != 0;
}
} //end of ilrd_hrd33_bitarray

#endif //ILRD_HRD33_BITARRAY_HPP