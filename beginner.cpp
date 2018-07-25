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

void PrintImage()
{
	/* Prints the image, using the symbols of the ASCII table */

	int n[2][9]={
                 {201,203,203,203,203,203,203,203,187}, 
                 {200,202,202,202,202,202,202,202,188}
                 };
        for(int i=0; i<2;i++)  
        {
			for(int j=0;j<9;j++) cout<<static_cast<char>(n[i][j]); 
				cout<<endl;
        };
        cin.get();
}

void PrintHouse()
{
	/* Draws a house using symbols /, \, - and _ */

	// Simplest method
	cout << "\t   House:nn" << endl;
	cout << "\t     / \\\n";
	cout << "\t    /   \\\n";
	cout << "\t   /     \\\n";
	cout << "\t  /------ \\\n";
	cout << "\t / |    |  \\\n";
	cout << "\t   |    | \n";
	cout << "\t   |    | \n";
	cout << "\t   |____| \n";
	cout << endl << endl << endl;
	

	// Drawing using template	
	int dom [][10] = 
	{     
		{0,0,0,0,1,2,0,0,0,0},
		{0,0,0,1,0,0,2,0,0,0},
		{0,0,1,0,0,0,0,2,0,0},
		{0,1,3,3,3,3,3,3,2,0},
		{1,0,4,0,0,0,0,4,0,2},
		{0,0,4,0,0,0,0,4,0,0},
	    {0,0,4,0,0,0,0,4,0,0},
		{0,0,4,0,0,0,0,4,0,0},
		{0,0,4,0,0,0,0,4,0,0},
		{0,0,4,5,5,5,5,4,0,0}
	};
	
	// Used symbols
	const char symbol [6]={' ','/','\\','--','|','_'}; 

	// Draw the object
    for(int i=0;i<10;i++) // Rows
    {
		for(int j=0;j<10;j++) // Columns
        {
			switch(dom[i][j])
            {
				case 0: cout<<symbol[0];continue;
                case 1: cout<<symbol[1];continue;
                case 2: cout<<symbol[2];continue;
                case 3: cout<<symbol[3];continue;
                case 4: cout<<symbol[4];continue;
                case 5: cout<<symbol[5];continue;
			}
		}
		cout<<endl;
	}
	cin.get();
}
