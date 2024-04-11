#include <iostream> // cout

#include "rectangle_shape.hpp"

using namespace std;

namespace ilrd_hrd33_shape
{
void Rectangle::Draw() const
{
    Shape::Draw();
    cout<< "Rectangle" << endl;
}
} // end of namespace ilrd_hrd33_shape