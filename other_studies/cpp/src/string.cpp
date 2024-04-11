#include <cstddef> /* size_t */
#include <cstring> /* strcpy, strlen */

#include "string.hpp"

static char *AllocAndCopy(const char *str_)
{
	char *mem_chunk = new char[1 + strlen(str_)];
	strcpy(mem_chunk, str_);

	return mem_chunk;
}

namespace HRD_33
{
String::String(const char *str_) : m_str(AllocAndCopy(str_))
{}

String::String(const String& other_) : m_str(AllocAndCopy(other_.m_str))
{}

String::~String()
{
	delete[] m_str;
	m_str = NULL;
}

String& String::operator=(const String& other_)
{
	// handle self assignment
	char *temp = AllocAndCopy(other_.m_str);
	delete[] m_str;
	m_str = temp;

	return *this;
}

const char* String::Cstr() const
{
	return m_str;
}

size_t String::Length() const
{
	return strlen(m_str);
}
} // namespace HRD_33
