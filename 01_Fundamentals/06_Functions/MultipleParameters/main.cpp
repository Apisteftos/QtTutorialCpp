#include <iostream>


using namespace std;


// with default parameter doesn't work
// void myFunction(string fname = "Konstantinos", int age);


/// @brief works perfect as a function
/// @param fname
/// @param age
void myFunction(string fname, int age)
{

    cout << fname << " Refsnes. " << age << " years old. \n";

}


int main()
{
    myFunction("Liam", 3);
    myFunction("Jenny", 14);
    myFunction("Anja", 30);

}

