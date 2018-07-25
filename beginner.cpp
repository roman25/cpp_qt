#include <iostream>

using namespace std;

void DividingNumberOfDigits()
{
	/* Reads the entered five-digit number. Each digit 
	of the number should be to display in a new line */

	// Define input data
	int inputNumber;
	cin >> inputNumber;
	cout << "Enter a number: " << inputNumber << endl;

	// Check range of input number
	if (inputNumber > 100000 || inputNumber < 10000)
	{
			cout << "Incorrect number" << endl;
			exit(1);
	}

	// Define counter
	int counter = 10000;

	// Define modulo
	int modulo = 0;

	while (counter > 0)
	{
		// Get modulo of a number
		modulo = inputNumber % counter;
		
		// Get differences between source and modulo
		int dif = inputNumber - modulo;

		// Update input number
		inputNumber = inputNumber - dif;
		int digit = dif / counter;

		cout << "digit is " << digit << endl;

		counter/=10;
	}
}

void CaclucalteMathematicalExpression()
{
	/* Caclulcates matematical expression */

	// Define input data
	int a, b, f, x;
	cout << "Enter digits: a, b, f: " << endl;
	
	cout << "a = ";
	cin >> a;
	cout << "b = ";
	cin >> b;
	cout << "f = ";
	cin >> f;

	cout << "Now is caclucalting (a + b - f / a) + f * a * a - (a + b)" << endl;
	x = (a + b - f / a) + f * a * a - (a + b);
	cout << "x = " << x << endl;
}

void ChangeTextCase()
{
	/* Changes text case from lower to upper */

	// Define input data
	char symbol;
	cout << "Enter a symbol in lower case ";
	cin >> symbol;
	symbol = symbol -32;
	cout << "Upper case " << symbol << endl;
}

void ConvertMeterstoKm()
{
	/* Converts meters to kilometers*/

	// Define input data
	double meters;
	cout << "Enter meters ";
	cin >> meters;

	cout << meters << " is " << meters / 1000 << " kilometers" << endl;
}
