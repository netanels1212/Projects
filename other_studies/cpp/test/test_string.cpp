#include <iostream>

#include "string.hpp"

using namespace std;
using namespace HRD_33;

int main()
{
	String s1("hello");
	cout<<endl<<"s1 is: "<<s1.Cstr()<<endl<<endl;

	String s2("world!!!!");
	cout<<endl<<"s2 is: "<<s2.Cstr()<<endl<<endl;

	String s3(s1);
	cout<<endl<<"s3 is: "<<s3.Cstr()<<endl<<endl;

	s1 = s2;
	cout<<"s1 is: "<<s1.Cstr()<<endl<<endl;

	cout<<"s1 length is: "<<s1.Length()<<endl;
	cout<<endl<<"s1 is: "<<s1.Cstr()<<endl<<endl;

	return 0;
}