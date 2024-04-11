#include <iostream> //cout

#include "sharedptr.hpp"

using namespace std;
using namespace ilrd_hrd33;

class Base
{
public:
    Base(int val = 3) : m_data(val) {}

    virtual void Print()
    {
        cout << "Base print - m_data: " << m_data << endl;
    }

    virtual ~Base() {};

private:
    int m_data;
};

class Derived : public Base
{
public:
    Derived(int val = 5) : m_blutiblut(val) {};

    virtual void Print()
    {
        cout << "Derived print - m_blutiblut: " << m_blutiblut << endl;
    }

    virtual ~Derived() {};

private:
    int m_blutiblut;
};

int main()
{
    //Creating a SharedPointer to Base
    SharedPointer<Base> basePtr1(new Base(8));
    SharedPointer<int> intPtr;

    //Creating another SharedPointer to base by basePtr
    SharedPointer<Base> basePtr2 = basePtr1;

    //Using dereference operator to access object
    (*basePtr1).Print();
    (*basePtr2).Print();
    cout << endl;

    //Using arrow operator to access object
    basePtr1->Print();
    basePtr2->Print();
    cout << endl;

    //Check upper cast
    SharedPointer<Derived> derivedPtr1(new Derived());
    SharedPointer<Base> basePtr3 = derivedPtr1;
    basePtr3->Print();
    cout << endl;

    return 0;
}
