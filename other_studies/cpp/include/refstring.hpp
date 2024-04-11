/*****************************************************************************/
/*
Project: String
Name: Netanel shmuel
Reviewer: Diana
Date: 01.02.2024
Version: 1.0 - Initial
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
public:
    String(const char *str_); // Implicit Ctor
    String(const String &other_);
    String &operator=(const String &other_);
    char operator[](size_t) const;
    char &operator[](size_t);
    ~String();
    size_t Length() const;
    char *Cstr() const;
private:
    details::CountedResource *m_cr;
};
} // end of ilrd_hrd33

#endif /*ILRD_HRD33_STRING_HPP*/