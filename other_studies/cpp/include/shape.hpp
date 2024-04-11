/*****************************************************************************/
/*
Project: Shape
Name: Netanel shmuel
Reviewer: ***
Date: 08.02.2024
Version: 1.0 - Initial        
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_SHAPES_HPP
#define ILRD_HRD33_SHAPES_HPP

#include <cstddef> // size_t
#include "utils_cpp.hpp"

namespace ilrd_hrd33_shape
{
//use genereted cctor, assignment operator
class Shape
{
public:
explicit Shape();
virtual ~Shape()NOEXCEPT;
virtual void Draw() const = 0;
Shape& Move(size_t );
private:
size_t m_offset;
};
} // end of ilrd_hrd33_shape

#endif // ILRD_HRD33_SHAPES_HPP

