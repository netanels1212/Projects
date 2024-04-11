#include <iostream>
#include "refstring.hpp"

using namespace std;
using namespace ilrd_hrd33;

int main()
{
    // Test for ctor and Cstr()
    String s1("Hello");
    cout << "s1: " << s1.Cstr() << endl;

    // Test for cctor
    String s2(s1);
    cout << "s2: " << s2.Cstr() << endl;

    // Test for operator=
    String s3 = s1;
    cout << "s3: " << s3.Cstr() << endl;

    // Test for operator[]
    cout << "s1[1]: " << s1[1] << endl;

    // Test for operator+=
    s1 += " World";
    cout << "s1 after +=: " << s1.Cstr() << endl;

    // Test for operator+
    s2 = s1 + "!";
    cout << "s2 after +: " << s2.Cstr() << endl;

    return 0;
}