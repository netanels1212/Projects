#include <new> // operator new
#include <cstring> // strlen, strcpy, strcat

#include "string_details.hpp"
#include "refstring.hpp"

namespace ilrd_hrd33
{
using namespace details;
String::String(const char *str_) : m_cr(CountedResource::Create(str_)) {}

String::String(const String &other_) : m_cr(other_.m_cr)
{
    m_cr->IncrementCounter();
}

String& String::operator=(const String &other_)
{
    // handle self assign
    if (this != &other_)
    {
        m_cr->DecrementCounter();
        m_cr = other_.m_cr;
        m_cr->IncrementCounter();
    }
    
    return *this;
}

char String::operator[](size_t index) const
{
    return (m_cr->GetStr())[index]; 
}

char& String::operator[](size_t index)
{
    CountedResource *temp = CountedResource::Create(m_cr->GetStr());
    m_cr->DecrementCounter();
    m_cr = temp;

    return (m_cr->GetStr())[index];
}

String::~String()
{
    m_cr->DecrementCounter();
}

size_t String::Length() const
{
    return strlen(m_cr->GetStr());
}

char* String::Cstr() const
{
    return m_cr->GetStr();
}

bool operator==(const String &s1_, const String &s2_)
{
    return (0 == strcmp(s1_.Cstr(), s2_.Cstr()));
}

bool operator!=(const String &s1_, const String &s2_)
{
    return !(s1_ == s2_);
}

String &operator+=(String& s1_, const String &s2_)
{
    char *newStr = new char[s1_.Length() + s2_.Length() + 1];
    strcpy(newStr, s1_.Cstr());
    strcat(newStr, s2_.Cstr());

    s1_ = newStr;

    delete[] newStr;

    return s1_;
}

const String operator+(String s1_, const String &s2_)
{  
    return s1_ += s2_;      
}
}// end of ilrd_hrd33


namespace details
{
CountedResource::CountedResource(const char *s_) : m_counter(1)
{
    strcpy(m_str, s_);
}

CountedResource* CountedResource::Create(const char* s_)
{
    size_t strSize = strlen(s_) + 1;
    void *mem_chunk = operator new(sizeof(size_t) + strSize);
    CountedResource* cr = new (mem_chunk) CountedResource(s_);

    return cr;
}

CountedResource::~CountedResource() {}

char* CountedResource::GetStr()
{
    return m_str;
}

void CountedResource::IncrementCounter()
{
    ++m_counter;
}

void CountedResource::DecrementCounter()
{
    --m_counter;

    if (0 == m_counter)
    {
        delete this;
    }
}

CountedResource* CountedResource::SplitResource()
{
    --m_counter; // my TODO: DecrementCounter(); ?

    return Create(m_str);
}

}// end of namespace details