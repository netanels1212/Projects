#include <iostream> // cout

#include "line_shape.hpp"

using namespace std;

namespace ilrd_hrd33_shape
{
void Line::Draw() const
{
    Shape::Draw();
    cout<< "Line" << endl;
}
} // end of namespace ilrd_hrd33_shape