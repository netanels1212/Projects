#include <iostream>

#include "string_details.hpp"
#include "proxy_refstring.hpp"

using namespace ilrd_hrd33;
using namespace std;

int main()
{
    String s1("Hello");
    String s2(s1);
    String s3 = "World!";
    String s4 = s1;

    cout << "s1 is: " << s1.Cstr() << endl << "str adrr: " << (size_t)s1.Cstr() << endl << "len: " << s1.Length() << endl << endl;
    cout << "s2 is: " << s2.Cstr() << endl << "str adrr: " << (size_t)s2.Cstr() << endl << "len: " << s2.Length() << endl << endl;
    cout << "s3 is: " << s3.Cstr() << endl << "str adrr: " << (size_t)s3.Cstr() << endl << "len: " << s3.Length() << endl << endl;
    cout << "s4 is: " << s4.Cstr() << endl << "str adrr: " << (size_t)s4.Cstr() << endl << "len: " << s4.Length() << endl << endl;

    char c = s3[2];
    s1[1] = 'a';
    s2[2] = 'b';
    s3[3] = 'c';
    s3[2] = s3[1];
    s4[2] = 'd';

    cout << "s1 is: " << s1.Cstr() << endl << "str adrr: " << (size_t)s1.Cstr() << endl << "len: " << s1.Length() << endl << endl;
    cout << "s2 is: " << s2.Cstr() << endl << "str adrr: " << (size_t)s2.Cstr() << endl << "len: " << s2.Length() << endl << endl;
    cout << "s3 is: " << s3.Cstr() << endl << "str adrr: " << (size_t)s3.Cstr() << endl << "len: " << s3.Length() << endl << endl;
    cout << "s4 is: " << s4.Cstr() << endl << "str adrr: " << (size_t)s4.Cstr() << endl << "len: " << s4.Length() << endl << endl;
    cout << "c = s3[2] -> " << c << endl;

    return 0;
}
