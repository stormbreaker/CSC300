#include <iostream>
#include <cstdlib>
using namespace std;
int main()
{
	int num;
	cout << "Hello world" << endl;
	cout << "Enter a number between 1 and 10: ";
	cin >> num;
	if (num < 1 || num > 10)
	{
		cerr << "Invalid Number" << endl;
	}
}

