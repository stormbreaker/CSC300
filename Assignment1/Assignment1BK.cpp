#include <iostream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <cctype>

using namespace std;
const int notrelprim = -999; // Honestly not sure this is necessary
int gcd(int a, int b); // basic gcd
void extended(int a, int b, int &j, int d); // extended Euclid
int main()
{
	int a,b,c,d,j,k,r,s,t,x,y,z;
	int xsmall, smult, temp;
	cout << "This program solves Diophantine equations of the form \n";
	cout << "ax + by = c" << endl;
	cout << "Enter a:  ";
	cin >> a;
	cout << "Enter b:  ";
	cin >> b;
	cout << "Enter c:  ";
	cin >> c;
	d = gcd(a,b);
	cout << d << endl;
	if (c % d != 0)
	{
		cout << "There is no solution" << endl;
	}
	else
	{
		r = c / d;
		extended(a, b, j, d);
		k = (d - (a * j)) / b;
		cout << j  << " " << k << endl;
		x = j * r;
		y = k * r;
		s = a / d;
		t = b / d;
		xsmall = x % t; //I believe this is proper
		smult = (x - xsmall) / t;
		y = y + (s * smult); 
		cout << xsmall << " " << y << endl;
	}
	return 0;	
}

int gcd(int a, int b)
{
	int r;
	// base case
	if (a == 0)
	{
		return b;
	}
	if (b == 0)
	{
		return a;
	}
	r = a % b;
	return gcd(b,r);
}

void extended(int a, int b, int &j, int d) // d is the value determined by
// the basic gcd function.  I pass this in as the check instead of 1
// // because Corwin's code assumes gcd is one (because of RSA)
{
	int k, q, r, r2, temp;
	if (a < b)
	{
		temp = a;
		a = b;
		b = temp;
	}

	if (b == 0)
	{
		j  = notrelprim;
		return;
	}
	q = a / b;
	r = a % b;

	if (r == d) //used to be if (r == 1)
	{
		j  = - q;
	}
	else
	{
		extended(b, r, j, d);
		if (temp == notrelprim)
		{
			j  = notrelprim;
			return;
		}
		k = (r * j - d) / b;
		j = - (q * j + k);
	}
	j = j % a;  // does this even actually do anything?
	if (j  < 0)
	{
		j = j + a;
	}
}
