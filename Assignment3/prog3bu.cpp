#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <queue>
#include <string>

using namespace std;

struct node
{
	int key;
	node *link = nullptr;
};

bool bubbleSort(vector<int> &array, int numItems);
void head_insert(node* head, int number);
void radixSort(vector<int> &array, int numItems);
void pause();
bool checkSorted(vector<int> &array);
void printFile(string sortName, ofstream &file, vector<int> &array, bool flag = true);

int main(int argc, char** argv)
{
	//int oarray[MAX]; //original array
	//int barray[MAX]; //array for bubble sort
	//int rarray[MAX]; //array for radix sort
	//int sarray[MAX]; //array for STL sort (is this necessary?)
	
	cout << setprecision(10) << fixed;

	int count = 0;
	int numItems;
	int temp;
	double seconds;

	vector<int> intVector; //original
	vector<int> bVector;
	vector<int> rVector;
	vector<int> sVector;

	intVector.clear();
	bVector.clear();
	rVector.clear();
	sVector.clear();
	cout << intVector.size();
	cout << bVector.size();
	cout << rVector.size();
	cout << sVector.size();
	



	time_t btimer, rtimer, stimer, currtime;

	bool flag;
	
	ifstream fin;
	ofstream fout, rout, sout, bout;
	fin.open(argv[1]);
	fout.open(argv[2]);
	//rout.open("radixlist.txt");
	//sout.open("stllist.txt");
	//bout.open("bubblelist.txt");

	if (!fin)
	{
		cout << "Error opening input file" << endl;
	}
	cout << "Success" << endl;
	
	if (!fout)
	{
		cout << "Error opening output file" << endl;
	}

	while (fin >> temp)
	{
		count++;
		intVector.push_back(temp);
	}
	cout << intVector.size() << endl;
	numItems = count - 1;
	bVector = intVector;
	rVector = intVector;
	sVector = intVector;	
	

	time(&btimer);
	flag = bubbleSort(bVector, numItems);
	time(&currtime);
	seconds = difftime(currtime, btimer);	

	/*for (int i = 0; i < numItems; i++)
	{
		bout << bVector[i] << endl;
	}*/
	flag = checkSorted(bVector);
	printFile("bubblesort", fout, bVector, flag);

	cout << "Bubble sort ran for: " << seconds << " seconds" << endl;
	
	time(&rtimer);
	radixSort(rVector, numItems);
	time(&currtime);
	seconds = difftime(currtime, rtimer);

	/*for (int i = 0; i < numItems; i++)
	{
		rout << rVector[i] << endl;
	}*/
	flag = checkSorted(rVector);
	printFile("radixsort", fout, rVector, flag);

	cout <<  "Radix sort ran for: " << seconds << " seconds" << endl;
	
	time(&stimer);
	sort(sVector.begin(), sVector.begin() + (numItems+1));//STL sort
	cout << sVector.size();
	time(&currtime);
	seconds = difftime(currtime, stimer);
	
	/*for (int i = 0; i < numItems; i++)
	{
		sout << sVector[i] << endl;
	}*/

	flag = checkSorted(sVector);

	printFile("STL sort", fout, sVector);

	cout << "STL sort ran for: " << seconds << " seconds" << endl;
	
	fin.close();
	fout.close();
	//bout.close();
	//rout.close();
	//sout.close();
	
}
bool bubbleSort(vector<int> &array, int numItems)
{	time_t start, end;
	double sec;
	time(&start);
	for(int it = 0; it < numItems; it++)
	{
		for (int i = 0; i < numItems - it; i++)
		{
			if (array[i] > array[i+1])
			{
				int temp = array[i];
				array[i] = array[i+1];
				array[i+1] = temp;
			}
			time(&end);
			if ((i + 1) % 1000 == 0 && difftime(end, start) >= 10)
			{
				return false;
			}
		}
	}
	return true;
}

void radixSort(vector<int> &array, int numItems)
{
	int temp = 0/* new (nothrow) int[numItems]*/;
	queue<int> list[256];
	int masks[4] = {255, 65280, 16711680, 2130706432};
	int* hold = new (nothrow) int[numItems];


	for (int i = 0; i < numItems + 1; i++)
	{
		hold[i] = array[i];
	}
	
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < numItems + 1; i++)
		{
			temp/*[i]*/ = hold[i] & masks[j];
			temp/*[i]*/ = temp/*[i]*/ >> (8 * j);
			list[temp/*[i]*/].push(hold[i]);	
		}
		int count = 0;
		for (int k = 0; k < 256; k++)
		{
			while (!list[k].empty())
			{
				hold[count] = list[k].front();
				list[k].pop();
				count++;		
			};
		} 
	}

	for (int i = 0; i < numItems + 1; i++)
	{
		array[i] = hold[i];
	}
}
/*void head_insert(node* head, int number)
{
	node* temp_ptr;
	temp_ptr = new node;
	temp_ptr->key = number;
	temp_ptr->link = head;
	//cout << head << endl;
	head = temp_ptr;
	//cout << head << endl;
}*/
void pause()
{
	char tempvar = 0;
	while(tempvar != 10)
	{
		tempvar = getchar();
	}
	tempvar = 0;
}
bool checkSorted(vector<int> &array)
{
	for (int i = 0; i < array.size(); i++)
	{
		if (i + 1 == array.size())
		{
			break;
		}
		else
		{
			if (array[i + 1] < array[i])
			{
				return false;
			}
		}
	}
	return true;
}
void printFile(string sortName, ofstream &file, vector<int> &array, bool flag)
{
	int spot = 0;
	file << sortName << ":" << endl;
	if (flag == false)
	{
		file << sortName << " took more than 10 seconds" << endl;
		return;
	}
	for (int i = 0; i < 10; i ++)
	{
		spot = i * (array.size() /10);
		file << "     ";
		file << array[spot] << endl;
	}
}
