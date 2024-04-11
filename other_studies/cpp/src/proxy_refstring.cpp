#include <new> // operator new
#include <cstring> // strlen, strcat
#include <algorithm> // copy

#include "string_details.hpp"
#include "proxy_refstring.hpp"

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
    other_.m_cr->IncrementCounter();
    m_cr->DecrementCounter();        
    m_cr = other_.m_cr;
        
    return *this;
}

char String::operator[](size_t index) const
{
    return (m_cr->GetStr())[index]; 
}

String::CharProxy String::operator[](size_t index)
{
    return CharProxy(this, index);
}

String::~String()NOEXCEPT
{
    m_cr->DecrementCounter();
}

size_t String::Length() const
{
    return strlen(m_cr->GetStr());
}

const char* String::Cstr() const
{
    return m_cr->GetStr();
}


String::CharProxy::CharProxy(String *str_, size_t index_) : m_proxStr(*str_), m_proxIndex(index_) {}

char String::CharProxy::operator=(char c_) 
{
    m_proxStr.m_cr = m_proxStr.m_cr->SplitResource();
    m_proxStr.m_cr->GetStr()[m_proxIndex] = c_;

    return c_;
}

char String::CharProxy::operator=(const CharProxy &prox_) 
{ 
    char c = prox_.m_proxStr.Cstr()[prox_.m_proxIndex];

    return operator=(c);
}

String::CharProxy::operator char() const 
{
    return m_proxStr.Cstr()[m_proxIndex];
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
    size_t len1 = s1_.Length();
    size_t len2 = s2_.Length();

    char *newStr = new char[len1 + len2 + 1];

    std::copy(s1_.Cstr(), s1_.Cstr() + len1, newStr);
    std::copy(s2_.Cstr(), s2_.Cstr() + len2 + 1, newStr + len1);

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
    std::copy(s_, s_ + strlen(s_) + 1, m_str);
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
    if(1 == m_counter)
    {
        return this;
    }
    
    --m_counter;

    return Create(m_str);
}

}// end of namespace details