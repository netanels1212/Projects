#include <iostream> // cout

#include "circle_shape.hpp"

using namespace std;

namespace ilrd_hrd33_shape
{
void Circle::Draw() const
{
    Shape::Draw();
    cout<< "Circle" << endl;
}
} // end of namespace ilrd_hrd33_shape