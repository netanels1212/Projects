#include <iostream> // cout
#include <cstddef> // size_t

#include "shape.hpp"
#include "utils_cpp.hpp"

using namespace std;

namespace ilrd_hrd33_shape
{
Shape::Shape() : m_offset(0) {}

Shape::~Shape()NOEXCEPT {}

void Shape::Draw() const
{
    cout<< string(m_offset, ' ');
}

Shape& Shape::Move(size_t offset_)
{
    m_offset = offset_;

    return *this;
}

} // end of namespace ilrd_hrd33_shape