#include <iostream> // cout

#include "square_shape.hpp"

using namespace std;

namespace ilrd_hrd33_shape
{
void Square::Draw() const
{
    Shape::Draw();
    cout<< "Square" << endl;
}
} // end of namespace ilrd_hrd33_shape