/********************************************************************************
Project: string
Name: Netanel Shmuel
Reviewer: 
Date: 28.1.2024
Version: 1.0
********************************************************************************/
#ifndef  ILRD_HRD33_STRING_HPP
#define ILRD_HRD33_STRING_HPP

#include <cstddef> /* size_t */

namespace HRD_33
{
class String
{
public:
    explicit String(const char *str_);
    String(const String& other_);
    ~String();
    String& operator=(const String& other_);

    const char* Cstr() const;
    size_t Length() const;

private:
    char *m_str;
};
} /* namespace HRD_33 */


#endif /* ILRD_HRD33_STRING_HPP */