#include <iostream>
#include "bitarray.hpp"

using namespace ilrd_hrd33_bitarray;
using namespace std;
int main()
{
    BitArray<65> a;
    BitArray<65> b;
    BitArray<65> c;
    BitArray<65> d;
    BitArray<65> e;
    BitArray<12> f;
    BitArray<12> g;

    a.Set();
    f.Set();
    g.Set();

    cout << "b(54) before |= - " << b.Get(54) << endl;
    b|=a;
    cout << "b(54) after |= - " << b.Get(54) << endl;

    cout << "c(3) before set bit - " << c.Get(3) << endl;
    c.Set(3);
    cout << "c(3) after set bit - " << c.Get(3) << endl;

    cout << "a(1) before flip bit - " << a.Get(1) << endl;
    a.Flip(1);
    cout << "a(1) after flip bit - " << a.Get(1) << endl;

    cout << "a(2) before flip all - " << a.Get(2) << endl;
    a.Flip();
    cout << "a(2) after flip all - " << a.Get(2) << endl;
    a.Set();
    cout << "d(10) before &= - " << d.Get(10) << endl;
    d.Set(10);
    d &= a;
    cout << "d(10) after &= - " << d.Get(10) << endl;
    cout << "d(10) before ^= - " << d.Get(10) << endl;
    d ^= a;
    cout << "d(10) after ^= - " << d.Get(10) << endl;
    d.Set();
    cout << "d==a - " << (d==a) << endl;
    cout << "c==a - " << (c==a) << endl;
    cout << "d!=a - " << (c==a) << endl;

    cout << "a count bits - " << a.CountBits() << endl;
    cout << "c count bits - " << c.CountBits() << endl;

    a[0] = true;
    a[0] = a[1];
    bool bla = a[3];
    a[2] = a[0];

    f = g;
    cout << a.ToString() << endl;
    cout << b.ToString() << endl;
    cout << d.ToString() << endl;
    cout << f.ToString() << endl;

    return 0;
}
