#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ofstream fout;
	int x;
	char *temp;
	temp = new (nothrow) char[sizeof(int)];
	cout << "Enter a number: ";
	cin >> x;
	fout.open("bintest.blah", ios::out | ios::binary | ios::app);
	
	fout.write((char*)&x, sizeof(int));
	return 0;
}
