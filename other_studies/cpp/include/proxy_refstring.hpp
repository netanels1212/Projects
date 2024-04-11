/*****************************************************************************/
/*
Project: String
Name: Netanel 
Reviewer: Diana
Date: 01.02.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_STRING_HPP
#define ILRD_HRD33_STRING_HPP

#include <cstddef> // size_t

#include "utils_cpp.hpp"
#include "string_details.hpp"

namespace ilrd_hrd33
{
class String;
bool operator==(const String &s1_, const String &s2_);
bool operator!=(const String &s1_, const String &s2_);
String &operator+=(String &s1_, const String &s2_);
const String operator+(String s1_, const String &s2_);


class String
{
private:
     class CharProxy;

public:
    String(const char *str_); // Implicit Ctor
    String(const String &other_);
    String &operator=(const String &other_);
    char operator[](size_t) const;
    CharProxy operator[](size_t);
    ~String()NOEXCEPT;
    size_t Length() const;
    const char *Cstr() const;
private:
    details::CountedResource *m_cr;
    
    // Uses generated ctor, dtor, cctor
    class CharProxy
    {
    public: 
        explicit CharProxy(String *str_, size_t index_);
        char operator=(char c_);
        char operator=(const CharProxy &prox_);
        operator char() const;
    private:
        String& m_proxStr;
        const size_t m_proxIndex;
    }; // end of class CharProxy
}; // end of class String
} // end of ilrd_hrd33

#endif /*ILRD_HRD33_STRING_HPP*/
