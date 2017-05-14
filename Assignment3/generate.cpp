/* Creator: Benjamin Kaiser
 * This program generates any number of random integers that fit in a
 * 32-bit signed int.  Output file and number of items are on the command
 * line.
 * Sample run:
 * gen MillionN.txt 1000000
 * compile using g++ -o gen generate.cpp
 */


#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>



using namespace std;

int main(int argc, char **argv)
{
	int num;
	ofstream fout;

	fout.open(argv[1], ios::ate);
	srand(time(NULL));

	int limit;
	limit = atoi(argv[2]);


	for (int i = 0; i < limit; i++)
	{
		num = rand() % 2147483647;
		fout << num << endl;
	}
	return 0;
	
}
