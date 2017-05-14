#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	ifstream fin;
	int x;
	fin.open(argv[1], ios::in | ios::binary);
	while(!fin.eof())
	{
		for (int i = 0; i < 6; i++)
		{
			fin.read((char*) &x, sizeof(int));
			cout << x << " ";
		}
		cout << endl;
	}
	fin.close();
}
