#include <iostream>
#include <string>

using namespace std;


// If I decrare twice the function it won't work with the default paramater.
// So then I have to declare my function at the header and not at the bottom
//void myFunction(string country = "Norway");


/// @brief this function works perfect with the requirements
/// @param country
void myFunction(string country = "Norway")
{
    cout << country << "\n";
}


int main()
{
    myFunction("Sweden");
    myFunction("India");
    myFunction();
    myFunction("USA");
    return 0;
}


