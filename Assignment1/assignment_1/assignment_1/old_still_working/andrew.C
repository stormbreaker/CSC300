// worked with Ben Kaiser

#include <iostream>
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

void my_euclid(big_num a, big_num b, big_num &j, big_num &k)
  {
  big_num x = 0;
  big_num y = 1;
  big_num x_l = 1;
  big_num y_l = 0;
  big_num q = 0;
  big_num swap;

  while (b != 0)
  {
	q = a / b;
	swap = b;
	b = a % b;
	a = swap;

	swap = x;
	x = x_l - q * x;
	x_l = swap;

	swap = y;
	y = y_l - q * y;
	y_l = swap;
  }

	j = x_l;
	k = y_l;

  return;
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
  
  while (counter > 0)
	{
		// ask user for input
		cout << endl;
		cout << "--------------------------" << endl;
		cout << "Enter a: ";
		cin >> a;
		cout << "Enter b: ";
		cin >> b;
		cout << "Enter c: ";
		cin >> c;
		
		cout << "a is: " << a << endl;
		cout << "b is: " << b << endl;
		
		// compute GCD
		d = euclid_gcd(a, b);
		
		
		cout << "The GCD is: " << d << endl;
		
		
		// check for a solution
		if (c % d != 0)
		{
			cout << "NO SOLUTION" << endl;
			return -1;
		}
		
		
		// compute r
		r = c / d;

		//----------FINE----------//
		
		
		// get j and k
		//euclid_algo_ex(a, b, j, k, x, y, x_prev, y_prev);
		my_euclid( a, b, j, k );
		
		cout << endl;
		cout << "a is: " << a << endl;
		cout << "b is: " << b << endl;
		cout << "j is: " << j << endl;
		cout << "k is: " << k << endl;
		cout << endl;
		
		// get x and y
		x = r * j;
		y = r * k;
		
		cout << "x is: " << x << endl;
		cout << "y is: " << y << endl;
		
		// get s and t
		s = a / d;
		t = b / d;
		
		cout << "s is: " << s << endl;
		cout << "t is: " << t << endl;
		
		// double check math here (may take to long)...
		if( x > 0 )
		{
		while( x > 0 )
		{
			x -= t;
			if( x < 0 )
			{
			x += t;
			break;
			}
		}
		}
		
		if( x < 0 )
		{
		while( x < 0 )
		{
			x += t;
			if( x > 0 )
			{
			break;
			}
		}
		}
		
		y = ( c - x * a ) / b;
		
		cout << "x is: " << x << endl;
		cout << "y is: " << y << endl;
		
		
				
		// output j and k, as well as a and b
		cout << "Case " << counter << ": " << a << "*" << x << " - " << b 
			<< "*" << y << " = " << c << endl;
		cout << endl;
		counter++;
	}

	return 0;
	}
