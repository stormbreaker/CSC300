// worked with Ben Kaiser

#include <iostream>
//#include <cmath>
using namespace std;

typedef long long big_num;

///////////////////////////////////////////////////////////////////////////////

big_num euclid_gcd(big_num a, big_num b)
  {
  if (b == 0)
  {
    return a;
  }

  a = euclid_gcd(b, a%b);
  }

///////////////////////////////////////////////////////////////////////////////

void swap_nums(big_num &num1, big_num &num2)
	{
	big_num swap;

	swap = num1;
	num1 = num2;
	num2 = swap;

	return;
	}

///////////////////////////////////////////////////////////////////////////////

void euclid_algo_ex(big_num a, big_num b, big_num &j, big_num &k, big_num &x,
	big_num &y, big_num &x_prev, big_num &y_prev)
	{
	big_num pass_next_x;
	big_num pass_next_y;

	if (b == 0)
	{
		j = x_prev;
		k = y_prev;
		return;
	}

	pass_next_x = x;
	x = x_prev - ( a / b ) * x;

	pass_next_y = y;
	y = y_prev - ( a / b ) * y;

	euclid_algo_ex(b, a % b, j, k, x, y, pass_next_x, pass_next_y);
	}
///////////////////////////////////////////////////////////////////////////////

void get_smallest_x( big_num &x, big_num t )
  {
  
  x %= t;
  
  if( x > 0 )
  	{
  		return;
  	}
  
  if( x < 0 )
  	{
  		x += t;
  	}
  }

///////////////////////////////////////////////////////////////////////////////

int main()
	{
	// Varible declarations 
	big_num a;
	big_num b;
	big_num c;
	big_num d;
	big_num j;
	big_num k;
	big_num r;
	big_num s;
	big_num t;
	big_num x = 0;
	big_num y = 1;
	big_num x_prev = 1;
	big_num y_prev = 0;
	int counter = 1;
	bool swap_flag = false;
  
  while (cin >> a >> b >> c)
	{
		cout << a << " " << b << " " << c << endl;
		// compute GCD
		d = euclid_gcd(a, b);
				
		// check for a solution
		if (c % d != 0)
		{
			cout << "NO SOLUTION" << endl;
			return -1;
		}
		
		
		// compute r
		r = c / d;

		// get j and k
		euclid_algo_ex(a, b, j, k, x, y, x_prev, y_prev);
				
		cout << "j is: " << j << endl;	
		cout << "k is: " << k << endl;		
		
		// get x and y
		x = r * j;
		y = r * k;
		
		// get t
		t = b / d;
		
		// find smallest value of x
		get_smallest_x( x, t );
		
		// solve for y
		y = ( c - x * a ) / b;
		
		// output solution a * x + b * y = c
		cout << "Case " << counter << ": " << a << "*" << x << " - " << b 
			<< "*" << y << " = " << c << endl;
			
		// increase counter
		counter++;
	}

	return 0;
	}
