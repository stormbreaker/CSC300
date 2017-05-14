/******************************************************************************
Name: Linux and Solving ax + by = c (Program 1)
Author: Andrew Fagrey and Ben Kaiser
Class: CSC300 Section M001 - Data Structers - Spring 2015
Instructor: Dr. Ed Corwin
Due date: February 12, 2015

Description: This program was designed to solve the equation ax + by = c for x 
	and y when given a, b, and c. The program encorporates math concepts such as 
	Euclid's algorithm. The program was written to be used at the command line 
	and accepts input via standard cin and outputs via standard cout. The program
	does not require any special files to run. It only requires that the user 
	enter a, b, and c. 

External resources needed: None

Compilation or linking instructions: None

Program errors: To do...

******************************************************************************/

#include <iostream>
#include <cmath>

using namespace std;

typedef long long big_num;


/******************************************************************************

	FUNCTION PROTOTYPES

******************************************************************************/
big_num euclid_gcd(big_num a, big_num b);
void euclid_ext( big_num a, big_num b, big_num &j, big_num &k );
void get_smallest_x( big_num &x, big_num t );


/******************************************************************************

	Name: main
	Type: int
	Parameters: 
			None
	Description: This is the starting point to the program and is the function in
		charge of calling the other functions of this program. The function reads 
		in input from the command prompt and proceeds to calculate x and y in the 
		equation ax + by = c. The program relys on the user to supply a, b, and c.
		The program will take input from the command line and then comput the 
		greated common divisor of a and b. The program then checks whether there is
		a solution to the problem. If there is no solution, the program continues 
		out of the loop and on to the next test case. The program will continue to 
		evalutate test cases until the user desides to quit the program.

******************************************************************************/
int main()
	{
	// Varible declarations 
	big_num a; 				  // used in eqaution
	big_num b; 			 	  // used in eqaution
	big_num c; 			  	// used in eqaution
	big_num d; 				  // used in eqaution
	big_num j;          // used to find possible solutions
	big_num k;          // used to find possible solutions
	big_num r;          // used to find possible solutions
	big_num t;          // used to find possible solutions
	big_num x;					// used to find smallest solution > 0
  big_num y;				  // used to find corresponding smallest solution > 0
	int counter = 1;    // loop counter
	
	// start of loop to evalutate test cases
  while (cin >> a >> b >> c) // read in values from command line
	{
		// compute GCD
		d = euclid_gcd(a, b);
		
		// check for a solution
		if ( c % d != 0 )
			{
			// if no solution exits, output no solution and continue on
			cout << "Case " << counter << ": " <<"NO SOLUTION" << endl;
			counter++;  // increase count so number of cases is correctly recorded
			continue;  // skip back up to start of loop if no solution
			}	// end if
		
		// compute r
		r = c / d;
		
		// get j and k
		euclid_ext( a, b, j, k );
		
		// get x (no need to compute y, solves for y directly)
		x = r * j;
		
		// get t (no need to compute s, will solve for y directly)
		t = b / d;
		
		// find smallest x value
		get_smallest_x( x, t );
		
		// solve for y based on smallest x value
		y = ( c - x * a ) / b;
		
		// output j and k, as well as a and b (no solution case outputed above)
		cout << "Case " << counter << ": " << a << " * " << x << " - " << b 
			<< " * " << abs(y) << " = " << c << endl;
		
		// increment counter for loop
		counter++;
	} // end while

	// program really never encouters the return, but it's here anyway
	return 0;
	}


/******************************************************************************

	Name: euclid_gcd
	Type: big_num
	Parameters: 
			big_num a			- value entered by user
			big_num b			- value entered by user
	Description: This function computes the greatest common divisor of two 
		numbers. The numbers are passed in and the GCD of the two numbers is 
		calculated recursively and then returned. This function is based on the 
		idea that the recursive call will always end at the basis case.

******************************************************************************/
big_num euclid_gcd(big_num a, big_num b)
  {
  // basis case - when b is zero, return a and move back up!
  if ( b == 0 )
  	{
    return a;
  	} // end if
	
	// if b isn't zero yet, call the function again.
  a = euclid_gcd( b, a % b );
  }
 

/******************************************************************************

	Name: euclid_ext
	Type: void
	Parameters: 
			big_num a			- value entered by user
			big_num b			- value entered by user
			big_num j			- value that completes modified equation aj + bk = d (this
											value is passed by reference)
			big_num k			- value that completes modified equation aj + bk = d (this
											value is passed by reference)
	Description: This function is responsible for finding j and k in the modified
		expression aj + bk = d. It solves for j and k iteratively. The method for 
		this algorithm was contructed with help from:  
		www.thefullwiki.org/Extended_Euclidean_algorithm 

******************************************************************************/
void euclid_ext(big_num a, big_num b, big_num &j, big_num &k)
  {
  // Variable declarations
  big_num x = 0;   // current x value
  big_num y = 1;   // current y value
  big_num x_prev = 1; // previous x value
  big_num y_prev = 0; // previous y value
  big_num swap; // swap variable

  while ( b != 0 )
		{
		// compute x value and next x value
		swap = x;
		x = x_prev - ( a / b ) * x;
		x_prev = swap;
		
		// compute y value and next y value
		swap = y;
		y = y_prev - ( a / b ) * y;
		y_prev = swap;
		
		// get next a and b values
		swap = b;
		b = a % b;
		a = swap;
		}
	
	// set j and k equal to previous x and y values
	j = x_prev;
	k = y_prev;

  return;
  }


/******************************************************************************

	Name: get_smallest_x
	Type: void
	Parameters: 
			big_num x			- equation value to be decreased
			big_num t			- computed in main function
	Description: This function is responsible for computing the smallest possible
		x value that makes the equation true. Note: there are other values that 
		make the equation true, but program specifications were to find the 
		smallest possible positive x value. This function also operates under the
		impression that x will never be a negative number.

******************************************************************************/
void get_smallest_x( big_num &x, big_num t )
  {
  // compute smallest x value by moding with t
  x %= t;
  
  // check that x is still positive
  if( x > 0 )
  	{
  	return;
  	}
  
  // if x is not positive, add t to x to get above 0
  x += t;  
  }
  
  
