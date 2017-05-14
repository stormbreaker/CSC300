#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

struct node
{
	int key;
	node *link;
};

void bubbleSort(vector<int> &array, int numItems);
void head_insert(node* head, int number);
void radixSort(vector<int> &array, int numItems);


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

	time_t btimer, rtimer, stimer, currtime;

	ifstream fin;
	ofstream fout;
	fin.open(argv[1]);
	fout.open(argv[2]);

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
	numItems = count - 1;
	bVector = intVector;
	rVector = intVector;
	sVector = intVector;	
	

	time(&btimer);
	bubbleSort(bVector, numItems);
	time(&currtime);
	seconds = difftime(currtime, btimer);	

	cout << "Bubble sort ran for: " << seconds << " seconds" << endl;
	
	time(&rtimer);
	radixSort(rVector, numItems);
	time(&currtime);
	seconds = difftime(currtime, rtimer);

	cout <<  "Radix sort ran for: " << seconds << " seconds" << endl;
	
	time(&stimer);
	sort(sVector.begin(), sVector.begin() + (numItems+1));//STL sort
	time(&currtime);
	seconds = difftime(currtime, stimer);


	cout << "STL sort ran for: " << seconds << " seconds" << endl;
	
	fin.close();
	fout.close();
	
}
void bubbleSort(vector<int> &array, int numItems)
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
				return;
			}
		}
	}
}

void radixSort(vector<int> &array, int numItems)
{
	int *temp = new (nothrow) int[numItems];
	node *list[256];
	int masks[4] = {255, 65280, 16711680, 4278190080};
	int* hold = new (nothrow) int[numItems];

	for (int i = 0; i < 256; i ++)
	{
		list[i] = nullptr;
	}

	for (int i = 0; i < numItems; i++)
	{
		hold[i] = array[i];
	}

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < numItems; i++)
		{
			temp[i] = hold[i] & masks[j];
			temp[i] = temp[i] >> (8 * j);	
			head_insert(list[temp[i]], array[i]);
		}
		int count = 0;
		for (int k = 0; k < numItems; k++)
		{
			while (list[k] != nullptr)
			{
				hold[count] = list[k]->key;//seg faults on this line
				list[k] = list[k]->link;//or this line
			
				count++;
			
			};
		} 
	}

	for (int i = 0; i < numItems; i++)
	{
		array[i] = hold[i];
	}

}
void head_insert(node* head, int number)
{
	node* temp_ptr;
	temp_ptr = new node;
	temp_ptr->key = number;
	temp_ptr->link = head;
	head = temp_ptr;
}
