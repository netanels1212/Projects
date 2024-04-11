#include <iostream> // cout
#include <cstddef> // size_t

#include "shape.hpp"
#include "circle_shape.hpp"
#include "rectangle_shape.hpp"
#include "square_shape.hpp"
#include "line_shape.hpp"

using namespace std;
using namespace ilrd_hrd33_shape;

int main()
{
    Circle c;
    Rectangle r;
    Square s;
    Line l;
    Shape *shapes[4] = {&c, &r, &s, &l};
    size_t i = 4;

    c.Draw();
    c.Move(8);
    c.Draw();
    c.Move(7).Draw();

    r.Draw();
    r.Move(6);
    r.Draw();
    r.Move(5).Draw();

    s.Draw();
    s.Move(4);
    s.Draw();
    s.Move(3).Draw();

    l.Draw();
    l.Move(2);
    l.Draw();
    l.Move(1).Draw();

    cout<< endl;

    for (i = 0; i < 4; ++i)
    {
        shapes[i]->Move(i);
        shapes[i]->Draw();
    }

    return 0;
}